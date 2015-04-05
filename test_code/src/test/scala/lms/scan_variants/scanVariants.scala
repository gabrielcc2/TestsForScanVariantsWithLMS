/*******************************************************************************************************************************************
ScalaVariantTests class, defining some initial tests for developing a code-generating scheme for scan variants,
based on EPFL's Light-weight modular staging library (with minor extensions) and a slightly modified version of their 
DSL api for C-code generation.

Author: 
Gabriel Campero
gabrielcampero@acm.org

LMS source code by Tiark Rompf and others, retrieved on December, 2014, from: http://scala-lms.github.io/

References: 
Klonatos, Yannis, et al. "Legobase: Building efficient query engines in a high-level language, 2014."

OvGU, January-April 2015.
*********************************************************************************************************************************************
More information on LMS:
Lightweight Modular Staging (LMS) is a runtime code generation approach. 
The framework provides a library of core components for building high performance code generators and embedded compilers in Scala.

LMS is used by several other projects, including:

Delite: domain-specific languages for heterogeneous parallel computing.
Spiral: library generators for high-performance numerical kernels.
LegoBase: query compilation in database systems.

LMS is developed and used by researchers and practitioners from EPFL, Stanford, ETH, Purdue, University of Rennes, Oracle Labs, Huawei Labs, 
and other institutions.

*********************************************************************************************************************************************
Notes on variants applied:
For clarity purposes, we decided implementing the loop optimizations using a decorator pattern. In every different loop optimization we
decide which former functions to inherit or override. The application of variants is commutative. 

	BRANCH REMOVAL: Only one call of remove branching is effectful. More calls dont add anything new to the execution.

	LOOP UNROLLING: Loop unrolling can be added up. If there is an attempt to use an unrolled loop but passing less tuples
    than there are instructions in one unrolled iteraton, then the whole loop will run regularly in a section defined as the residue of the unrolling. 
    When combining with vectorization, only the inner vectorized loop is unrolled (the alignment and residual loops are not altered).

	THREAD-LEVEL PARALLELIZATION: As it is implemented, only one parallelization is defined, dividing the iterations among them.
	Parallel-prefix sum is applied to avoid synchronization overheads. 
	Number of threads will be passed as parameter at run time.
	When parallelizing, unrolling is always performed inside the parallel sections.
	Adding more threads than there are iterations will have no effect, and the loop will run unparallelized in the section defined as the residue
    of the unrolling.
	
	VECTORIZATION: Only a vector of size 4 is supported in this version. Passing less tuples than the vector size will mean that the
	iterations will be processed in the alignment section.
	
	GPU PARALLELIZATION: At this point only stand-alone GPU parallelization has been tested (without combining with other
	variants). It will be released soon as an early test of the expressive capacities of LMS.

*********************************************************************************************************************************************
Future work:
- Embed generator in cleaner function, taking input from console, so as to be usable by an existing system.
- Design and apply a clear scheme for naming the generated codes according to variants used, or to other requirements from existing system.
- A template type could also be used for the input variables.
- In order to support vectorization, some functions for mapping were implemented in the scala.virtualization.lms.common.MiscOps file.
  However they were implemented without reflection (since using that would imply using Rep[] as input, which then would be mapped to
  C with quotation marks or with the Const() transformator). It would be of interest to evaluate if the reflective effects would be needed, if so,
  the Rep[] input should be used, and the small errors of using quotation marks of the Const() transformator have to be addressed by post-processing 
  the generated code, or another method.
- Generate parallelism not through post-processing but through defining staged operators and emitting a function 
(https://github.com/TiarkRompf/virtualization-lms-core/blob/develop/src/common/Functions.scala), albeit inlined at call site. 
Perhaps a workaround for this inlining issue could be implemented, so as to make the generated accepted by more compilers.

Upcoming work:
    	     * Paper
    	     * Results for comparison using CoGaDB. 

Notes on testing:
   			 * Some simple, yet combinatorial-exhaustive testing with the 4 variants in different orders was carried out as a proof of validity of
   			   this code-generating library (please note that the examples of the code generated for this are included in the repository). 
   			   However, the possible effects of variations in the unrollDepth or number of tuples, were not tested.
   			   If there are possible bugs in our proposal, they might relate to those un-tested cases.
   			   
*********************************************************************************************************************************************
*/

package scala.lms.scan_variants

import scala.virtualization.lms.common._
import scala.io.Source
import java.io.PrintWriter
import java.io.File

class ScanVariantsTests extends TutorialFunSuite {
  
  /***********************************************************************************************************
   1. PRE-PROCESSING SECTION- DEFINITIONS
    ***********************************************************************************************************/
  
  val under = "c_code_generation_tests_"
  
  test("ScanVariants") {
    
    /**Future work: This should be embedded in a cleaner function, 
     * taking input from console, so as to be usable by an existing system. **/
    
    /*Configuration for the variants to be applied*/
    def numVariantsToApply: Int=4
    
    /**Example instructions:
     * "Remove branching"
     * "Unroll"
     * "Parallelize"
     * "Vectorize"
     * "_" (do nothing)
     * */
    var instructions = new Array[String](numVariantsToApply) 
    instructions(0) = "Remove branching"
    instructions(1) = "Parallelize"
    instructions(2) = "Vectorize"
    instructions(3) = "Unroll"

    /**Aditional variables for configuration (unrollDepth and vectorSize) can be changed further ahead, next to the declaration
     * of the loop object, in the processing section.*/
      
    /**Configuration for the predicate
     * Could be EQUAL, GREATER, GREATER_EQUAL, LESSER, LESSER_EQUAL, 
     * NOT_EQUAL. 
     * Anything else would mean select all.**/    
    def predicateAssigned: String = "GREATER_EQUAL" 
    						  
    /*Definition of the snippet for code-generation, using a slightly modified version of EPFL's LMS DSL api for C-code generation*/		
    val snippet = new DslDriverC[Array[Float],Unit] {
          
    	/*Area where context is shared between 2 stages: the code-generation stage and the execution stage*/
    	
    	/**Snippet function defining the function whose code is to be generated
    	 * Takes as input an array of floats. By convention, the first item is the value for comparison, 
    	 * the second item is the size of the input array or number of tuples, the  third the number of threads. 
    	 * The next 2*(number of threads) spaces are used by the generator. 
    	 * The following number of tuples items are the input, and the last number of tuples items are where the 
    	 * output will be stored. 
    	 * In this way we pack both the input and the needed memory to be allocated. 
    	 * 
    	 * An array following this scheme is generated and initialized in the preceeding part of 
    	 * the code, which is defined in the dsl api ...**/

      def snippet(input: Rep[Array[Float]]) = comment("Scan Variants", verbose = true) {
        
    	/*Input values*/
    	def valueForComparison:Rep[Float]=input(0) 
	    	
    	def maxNumIt:Rep[Int]=input(1).asInstanceOf[this.Rep[Int]]
    		
    	def numThreadsSelected:Rep[Int]=input(2).asInstanceOf[this.Rep[Int]]
	  
    	/*Local context variables, used for initialization of variables*/
    	var initialOutputPos: this.Variable[Int]=0 //Definition of the initial output position, relative to number of hits.
	  		
    	val zero: Rep[Int]=varIntToRepInt(initialOutputPos) //Definition of number 0, for typing purposes.
    	  	        		
    	/*Definition of the AbstractLoop class, in charge of handling the application of the variants. */
    	abstract class AbstractLoop {
    	  
    	  /*ATTRIBUTES*/
    	    
    	  /*Main attributes (functions as values)*/
    			
    	  def value: Rep[Float]//Value for comparison
    
    	  def numIterations: Rep[Int]//Number of interations of original loop
    			
    	  def predicate: String //Predicate
    			
    	  def outputPos: Var[Int] //Definition of the output position, relative to number of hits.
    			
    	  def numIterationsUnrolled: Rep[Int] //Number of Iterations of unrolled loop
    			
    	  def numThreads: Rep[Int]  //Number of threads
    			
    	  def numInst: Int  //Number of instructions per iteration. 1 by default.
    	      	   			
    	  /*Secondary attributes*/
    			
    	  def displacementForIterationPos: Rep[Int] /**Displacement for iteration position, 
    	   											* Allowing to map between logical iteration number
    	   											* and its position in the input array, following the
    	   											* our scheme. */ 
    			
    	  /*Flags*/
    			
    	  def bfc: Boolean //Flag indicating if branch free code was selected
    			
    	  def parallel: Boolean //Flag indicating if parallelization has been selected
    	  
       	  def vectorized: Boolean /**Flag indicating if vectorization has been selected. Used to avoid attempts 
       	                           * at several vectorizations (since its not supported)*/

    	  /*FUNCTIONS*/
    	  /*Default constructor*/
    	  def this (valueForComparison: Rep[Float], maxNumIt: Rep[Int], pred:String){
    	    this()//Nothing implemented, since its an abstract class
    	  }
    			
    	  /*Function for evaluating specific predicates*/
    	  def eval (val1: Rep[Float], val2:Rep[Float]): Rep[Boolean] ={
    	    predicate match {
    	      case "EQUAL" => val1==val2 
    	      case "GREATER" => val1>val2 
    	      case "GREATER_EQUAL" => val1>=val2 
    	      case "LESSER" =>  val1<val2
    	      case "LESSER_EQUAL" => val1<=val2 
    	      case "NOT_EQUAL" =>  val1!=val2 
    	      case _ => true
    		}
    	  }		
    			    						
    	  /*runInstructionOfIteration: Defines one single step of an iteration (w.r.t. the original iterations)*/
    	  def runInstructionOfIteration(it: Rep[Int], dispIt: Rep[Int])= comment("run instruction with branching", verbose = true){
    	    var itVal:Rep[Int]=it+dispIt
    	    if (eval(input(itVal),value.asInstanceOf[Rep[Float]])){ //Branching code
    	      input(varIntToRepInt(outputPos)+dispIt+numIterations)=(it).asInstanceOf[Rep[Float]]
    	      outputPos+=1
    		}
    	  }
 
    	  /*runIteration: All the steps to be performed during an interation*/
    	  def runIteration(it: Rep[Int], instOfIt:(Rep[Int],Rep[Int])=>Unit, dispIt:Rep[Int])= comment("run iteration as in simple loop", verbose = true){
    	    instOfIt(it, dispIt)
    	  }
    	  
    	   /** innerRunLoop: Inner function responsible for running the loop (in this case, a simple loop) 
    	   *  Returns the number of hits found.
    	   *  Takes as input the function in charge of the iteration, and the function in charge of the instruction per iteration*/
    	  def innerRunLoop(numIn:Int, numItU:Rep[Int], iteration:(Rep[Int],(Rep[Int], Rep[Int])=>Unit, Rep[Int])=>Unit, instOfIt:(Rep[Int],Rep[Int])=>Unit, dispIt:Rep[Int], isbfc: Boolean, isparallel:Boolean, numTh:Rep[Int]): Var[Int] = {
    	    /*Loop for code generation*/
    		for (i <- (0 until numIterations): Rep[Range]) {
    		  iteration(i, instOfIt, dispIt)
    		}
    		outputPos
    	  }
    			    			
    	  /** runLoop: Outer function calling the execution of the loop (in this case, a simple loop)  
    	   *  Returns the number of hits found.*/
    	  def runLoop(): Var[Int] = {
    	    innerRunLoop(numInst, numIterationsUnrolled, runIteration, runInstructionOfIteration, displacementForIterationPos, bfc, parallel, numThreads)
    	  }
    	  
       	}
    		
    	/*Simple loop class*/
    	class SimpleLoop (valueForComparison: Rep[Float], maxNumIt: Rep[Int], pred:String) extends AbstractLoop (valueForComparison, maxNumIt, pred){
    	  
    	  /*Over-written functions, representing the initialization of the values*/
    	  
    	  override def value = valueForComparison
    		  
    	  override def numIterations = maxNumIt
    		  
    	  override def predicate = pred 
    		  
    	  override def outputPos =initialOutputPos
    		  
    	  override def displacementForIterationPos= 3
    		  
    	  override def numIterationsUnrolled=maxNumIt
    		  
    	  override def numInst=1
    		  
    	  override def numThreads=1
    		  
    	  override def bfc=false
    		  
    	  override def parallel=false
    	  
    	  override def vectorized=false
    			
    	}
    		
    	/**Class following the decorator pattern, allowing us to add up the different
    	* optimizations*/
    	abstract class LoopDecorator (decoratedLoop: SimpleLoop)  extends SimpleLoop (decoratedLoop.value, decoratedLoop.numIterations, decoratedLoop.predicate) {
    	  
    	  /*Over-written functions, representing the default use of existing decorations*/
    		  
    	  override def outputPos= decoratedLoop.outputPos
    			
    	  override def displacementForIterationPos= decoratedLoop.displacementForIterationPos 
    			
    	  override def numIterationsUnrolled=decoratedLoop.numIterationsUnrolled
    			
    	  override def numInst=decoratedLoop.numInst
    			
    	  override def numThreads=decoratedLoop.numThreads
    			
    	  override def bfc=decoratedLoop.bfc
    			
    	  override def parallel=decoratedLoop.parallel
    	  
    	  override def vectorized=decoratedLoop.vectorized
    	  
    	  override def runInstructionOfIteration(it: Rep[Int], dispIt:Rep[Int])= comment("decorated instruction", verbose = true){
    	    decoratedLoop.runInstructionOfIteration(it, dispIt)
    	  }
    	  
    	  override def runIteration(it: Rep[Int], instOfIt:(Rep[Int],Rep[Int])=>Unit, dispIt:Rep[Int])= comment("decorated iteration", verbose = true){
    		decoratedLoop.runIteration(it, instOfIt, dispIt)
    	  }
    	  
    	  override def innerRunLoop(numIn:Int, numItU:Rep[Int], iteration:(Rep[Int],(Rep[Int],Rep[Int])=>Unit, Rep[Int])=>Unit, instOfIt:(Rep[Int],Rep[Int])=>Unit, dispIt:Rep[Int], isbfc: Boolean, isparallel:Boolean, numTh:Rep[Int]): Var[Int] = {
    	    decoratedLoop.innerRunLoop(numIn, numItU, iteration, instOfIt, dispIt, isbfc, isparallel, numTh)
    	  }
    			
    	  override def runLoop(): Var[Int]={
    	    innerRunLoop(numInst, numIterationsUnrolled, runIteration, runInstructionOfIteration, displacementForIterationPos, bfc, parallel, numThreads)
    	  }
    			
    	}
    		
    	/*Class implementing branch removal*/
    	class BranchFreeLoop (decoratedLoop: SimpleLoop) extends LoopDecorator (decoratedLoop) {
    		  
    	  override def bfc=true
    		  
    	  override def runInstructionOfIteration(it: Rep[Int], dispIt:Rep[Int])= comment("run instruction without branching", verbose = true){
    	    val itVal:Rep[Int]=it+dispIt
    		input(varIntToRepInt(outputPos)+dispIt+numIterations)=(it).asInstanceOf[Rep[Float]]
    		outputPos+=eval(input(itVal),value.asInstanceOf[Rep[Float]]).asInstanceOf[Rep[Int]]	
    	  }
    	  	  
    	}
    		
    	/*Class implementing loop unrolling*/
    	class UnrolledLoop (decoratedLoop: SimpleLoop, unrollDepth:Int) extends LoopDecorator(decoratedLoop){
    		  
    	  override def numIterationsUnrolled=((decoratedLoop.numIterationsUnrolled).asInstanceOf[Rep[Float]]/unrollDepth).asInstanceOf[Rep[Int]]
   			  
    	  override def numInst=(decoratedLoop.numInst*(unrollDepth.asInstanceOf[Int]))
  
    	  override def runIteration(it: Rep[Int], instOfIt:(Rep[Int],Rep[Int])=>Unit, dispIt:Rep[Int])= comment("run iteration from unrolled loop", verbose = true){
    		var currInst:Int=0
    		val baseIt:Rep[Int]=(it*numInst)
    		while (currInst<numInst){
    		  var itVal:Rep[Int]=currInst+baseIt
    		  instOfIt(itVal, dispIt)
    		  currInst+=1
    		} 
    	  }

    	  override def innerRunLoop(numIn:Int, numItU:Rep[Int], iteration:(Rep[Int],(Rep[Int], Rep[Int])=>Unit, Rep[Int])=>Unit, instOfIt:(Rep[Int], Rep[Int])=>Unit, dispIt:Rep[Int],isbfc: Boolean, isparallel:Boolean, numTh:Rep[Int]): Var[Int] = {
    	    if (isparallel|| decoratedLoop.vectorized){
    	      decoratedLoop.innerRunLoop(numInst, numIterationsUnrolled, runIteration, instOfIt, dispIt, isbfc, isparallel, numTh)
    	    }
    		else{
    			/*Loop for code generation*/
    			for (i <- (0 until numItU): Rep[Range]) {
    			  iteration(i, instOfIt, dispIt)
    		    }
    			/*Residual iterations*/
    			val maxIterationCovered:Rep[Int]=numItU*numIn
    			for (i <- (maxIterationCovered until numIterations): Rep[Range]) {
    			  instOfIt(i, dispIt)
    			}
    		    outputPos
    		}
    	  }
    	  
    	  override def runLoop(): Var[Int] = {
    	    innerRunLoop(numInst, numIterationsUnrolled, runIteration, decoratedLoop.runInstructionOfIteration, decoratedLoop.displacementForIterationPos, decoratedLoop.bfc, decoratedLoop.parallel, decoratedLoop.numThreads)
    	  }
    		  
    	}
    		
    	/*Class implementing loop parallelization*/
    	class ParallelLoop (decoratedLoop: SimpleLoop) extends LoopDecorator(decoratedLoop){
    			
    	  override def numThreads=numThreadsSelected

    	  override def parallel=true
    			
    	  override def displacementForIterationPos= {
    	    if (!decoratedLoop.parallel){
    	      decoratedLoop.displacementForIterationPos+(2*numThreads)
    		}
    		else{
    		  decoratedLoop.displacementForIterationPos			    
    		}
    	  }
    			  			    			
    	  /**Function that counts the outputs of a thread.
    	   * Takes as input the thread number.
    	   * It handles mapping from thread number to iteration number to input & output arrays, 
    	   * considering variants performed.**/
    	  def runParallelPrefixSum(it:Rep[Int], numIn:Int, numItU:Rep[Int], isbfc: Boolean)= comment("parallel prefix sum", verbose = true){
    	    var count: Variable[Int] = 0
    	    var baseVal: Rep[Int]= (displacementForIterationPos)+numIn*((numItU/numThreads).asInstanceOf[Rep[Int]]*it)
    	    for (i <- (0 until (numItU/numThreads).asInstanceOf[Rep[Int]]): Rep[Range]) {
    	      var itVal:Rep[Int]=baseVal+(i*numIn)
    	      var currInst:Int=0
    	      while (currInst<numIn){
    	        if (isbfc){ //Branch-free code... To avoid using this flag we would have to create yet another function: count...
    	          count+=eval(input(itVal),value.asInstanceOf[Rep[Float]]).asInstanceOf[Rep[Int]]
    			}
    			else {//Branching code
    			  if (eval(input(itVal),value.asInstanceOf[Rep[Float]])){
    			    count+=1				
    			  }
    			}
    			currInst+=1
    			itVal+=1; 
    	      }
    	    }	    				
    		input(3+it)=varIntToRepInt(count).asInstanceOf[Rep[Float]] //We store the size of output array before returning.		
    	  } 			
    			
    	  /**Function that performs the parallel processing of a thread.
    	   * Takes as input the thread number.
    	   * It handles mapping from thread number to iteration number to input & output arrays, 
    	   * considering variants performed.**/
    	  def runParallelWriting(it: Rep[Int], numIn:Int, numItU:Rep[Int], isbfc: Boolean)= comment("parallel writing", verbose = true){
    	    var count: Variable[Int] = 0
    	    for (i <- (0 until (numItU/numThreads).asInstanceOf[Rep[Int]]): Rep[Range]) {
    	      var currInst:Int=0
    	      while (currInst<numIn){  
    	        var itVal:Rep[Int]=displacementForIterationPos
    			itVal+=currInst
    			itVal+=((i+((numItU/numThreads).asInstanceOf[Rep[Int]]*it))*numIn)
    			if (isbfc){//Branch-free code... To avoid using this flag we would have to create yet another function...
    			  input(input(3+it+numThreads).asInstanceOf[Rep[Int]]+displacementForIterationPos+numIterations+varIntToRepInt(count))=(itVal-displacementForIterationPos).asInstanceOf[Rep[Float]]
    			  count+=eval(input(itVal),value.asInstanceOf[Rep[Float]]).asInstanceOf[Rep[Int]]
    			}
    			else{//Branching code
    			  if (eval(input(itVal),value.asInstanceOf[Rep[Float]])){
    			    input(input(3+it+numThreads).asInstanceOf[Rep[Int]]+displacementForIterationPos+numIterations+varIntToRepInt(count))=(itVal-displacementForIterationPos).asInstanceOf[Rep[Float]]
    				count+=1
    			  }
    			}
    	        currInst+=1
    	      } 
    		}    				    				
    	  }    			
    			
    	  override def innerRunLoop(numIn:Int, numItUn:Rep[Int], iteration:(Rep[Int],(Rep[Int], Rep[Int])=>Unit, Rep[Int])=>Unit, instOfIt:(Rep[Int],Rep[Int])=>Unit, dispIt:Rep[Int], isbfc: Boolean, isparallel:Boolean, numTh:Rep[Int]): Var[Int] = {
    	    if (decoratedLoop.vectorized){
    	    	decoratedLoop.innerRunLoop(numIn, numItUn, iteration, instOfIt, displacementForIterationPos, isbfc, parallel, numThreads)
    	    }
    	    else{
    	      //Parallel prefix sum...
    	      for (j <- (0 until numThreads): Rep[Range]) {
    	        runParallelPrefixSum(j, numIn, numItUn, isbfc)
    	      }
    	      //Serial assignment of output positions...
    	      if (numThreads>0){
    	    	  var tempVal:Rep[Int]=3+numThreads
    	    	  input(tempVal)=zero.asInstanceOf[Rep[Float]]
    	    	  for (k <- (1 until numThreads): Rep[Range]) {
    	    	    input(tempVal+k)=input(3+k-1)+input(tempVal+k-1)
    	    	  }
    	    	  outputPos=(input(tempVal-1)+input(tempVal-1+numThreads)).asInstanceOf[Rep[Int]]
    	      }    
    	      //Parallel writing...
    	      for (l <- (0 until numThreads): Rep[Range]) {
    	        this.runParallelWriting(l, numIn, numItUn, isbfc)
    	      }
    	      //Residual iterations
    	      var maxIterationCovered:Rep[Int]=(numItUn/numThreads)*numIn*numThreads.asInstanceOf[Rep[Int]]
    	      for (i <- (maxIterationCovered until numIterations): Rep[Range]) {
    	        instOfIt(i, displacementForIterationPos)
    		  }
    	      outputPos
    	    }
    	  }
    	  
       }
    	
    		
    	/*Class implementing vectorization*/
    	class VectorizedLoop (decoratedLoop: SimpleLoop, vectorSize: Int) extends LoopDecorator (decoratedLoop) {
    	  
    	  override def vectorized=true
    	  
    	  /*Function for evaluating specific predicates, using x86 vectorized operators*/
    	  def evalVect (val1: Rep[Int], val2:Rep[Int]): Rep[Int] ={ 
      	     predicate match {
      	       case "EQUAL" => _mm_cmpeq_ps(val1, val2)
      	       case "GREATER" => _mm_cmpgt_ps(val1, val2)
    	       case "GREATER_EQUAL" => _mm_cmpge_ps(val1, val2) 
    	       case "LESSER" =>  _mm_cmplt_ps(val1, val2)
    	       case "LESSER_EQUAL" => _mm_cmple_ps(val1, val2)
    	       case "NOT_EQUAL" =>  _mm_cmpneq_ps(val1, val2) 
    	       case _ => _mm_cmpeq_ps(val1,val1) //Always true, not by operator but by parameters
    		 }
      	  }
    	  
    	  /*Function implementing bitwise compare for using x86 vectorized operators*/
    	  def vectorizedRunInstructionOfIteration(it: Rep[Int], dispIt:Rep[Int], compVal:Rep[Int], isbfc:Rep[Boolean])= comment("decorated instruction for vectorization", verbose = true){
    	    if (isbfc){
    	      input(varIntToRepInt(outputPos)+dispIt)=it.asInstanceOf[Rep[Float]]
              outputPos+=(bitwiseCompare(compVal,1))
    	    }
    	    else{
    	      if((bitwiseCompare(compVal,1)).asInstanceOf[Rep[Boolean]]){
    		        input(varIntToRepInt(outputPos)+dispIt)=it.asInstanceOf[Rep[Float]]
    		        outputPos+=1
    		  }
    	    }
    	  }
    	  
    	  def runVectorizedParallelPrefixSum(it:Rep[Int], numIn:Int, numItU:Rep[Int], isbfc: Boolean, numTh:Rep[Int], sse_array_length:Rep[Int], sse_array: Rep[Array[Float]])= comment("parallel prefix sum", verbose = true){
    	    var count: Variable[Int]=0
    	    var comp_val: Rep[Int]= _mm_set1_ps(value)
     	    var read_val: Rep[Int]= _mm_set1_ps(zero.asInstanceOf[Rep[Float]])
      	    var comp_result: Rep[Int]= _mm_set1_ps(zero.asInstanceOf[Rep[Float]])
    	    for (i<- (0 until sse_array_length/(numIn*numTh)): Rep[Range]){
    	      var currInst:Int=0
      	      while (currInst<numIn){
      	        var tempVal: Rep[Int]=(numIn*i)+((4*it/numTh))+currInst
      	        read_val=_mm_load_ps((referenceOperator(sse_array(tempVal)).asInstanceOf[Rep[Float]]).asInstanceOf[Rep[Array[Float]]])
      	        comp_result=evalVect(read_val, comp_val)//Vectorized comparison
    		    var mask: Rep[Int]= _mm_movemask_ps(comp_result)
    		    if (isbfc){
    		      count+=(bitwiseCompare(mask,1))
    		      count+=(bitwiseCompare(binaryRightShift(mask,1),1))
     		      count+=(bitwiseCompare(binaryRightShift(mask,2),1))
      		      count+=(bitwiseCompare(binaryRightShift(mask,3),1))
    		    }
    	        else{
    	          if((bitwiseCompare(mask,1)).asInstanceOf[Rep[Boolean]]){
    	            count+=1
    		      }
    	          if((bitwiseCompare(binaryRightShift(mask,1),1)).asInstanceOf[Rep[Boolean]]){
    	            count+=1
    		      }
    	          if((bitwiseCompare(binaryRightShift(mask,2),1)).asInstanceOf[Rep[Boolean]]){
    	            count+=1
    	          }
    	          if((bitwiseCompare(binaryRightShift(mask,3),1)).asInstanceOf[Rep[Boolean]]){
    	            count+=1
    	          }
    	        }
      	        currInst+=1
    	      }
    		}
    		input(3+it)=varIntToRepInt(count).asInstanceOf[Rep[Float]] //We store the size of output array
    	  }
    	  def runVectorizedParallelWriting(it: Rep[Int], numIn:Int, numItU:Rep[Int], isbfc: Boolean, offsetDisp:Rep[Int], numTh:Rep[Int], dispIt:Rep[Int], sse_array_length:Rep[Int], sse_array: Rep[Array[Float]])= comment("parallel writing", verbose = true){
    	    var count: Variable[Int]=0
    	    var comp_val: Rep[Int]= _mm_set1_ps(value)
     	    var read_val: Rep[Int]= _mm_set1_ps(zero.asInstanceOf[Rep[Float]])
      	    var comp_result: Rep[Int]= _mm_set1_ps(zero.asInstanceOf[Rep[Float]])
    	    val displacementVal: Rep[Int]= dispIt+numIterations
    	    var basetidVariable:Var[Int]=0+offsetDisp//Offset+displacement
    	    for (i<- (0 until (sse_array_length/(numIn*numTh))): Rep[Range]){
    	      var currInst:Int=0
      	      while (currInst<numIn){
      	        read_val=_mm_load_ps(referenceOperator(sse_array((numIn*i)+(4*it/numTh)+currInst).asInstanceOf[Rep[Float]]).asInstanceOf[Rep[Array[Float]]])
      	        comp_result=evalVect(read_val, comp_val)//Vectorized comparison
    		    var mask: Rep[Int]= _mm_movemask_ps(comp_result)
    		    if (isbfc){
           		    input(input(3+it+numTh).asInstanceOf[Rep[Int]]+varIntToRepInt(count)+displacementVal)=varIntToRepInt(basetidVariable).asInstanceOf[Rep[Float]]
    		        basetidVariable+=1
    		        count+=(bitwiseCompare(mask,1))
    		        input(input(3+it+numTh).asInstanceOf[Rep[Int]]+varIntToRepInt(count)+displacementVal)=varIntToRepInt(basetidVariable).asInstanceOf[Rep[Float]]
    		        basetidVariable+=1
     		        count+=(bitwiseCompare(binaryRightShift(mask,1),1))
           		    input(input(3+it+numTh).asInstanceOf[Rep[Int]]+varIntToRepInt(count)+displacementVal)=varIntToRepInt(basetidVariable).asInstanceOf[Rep[Float]]
    		        basetidVariable+=1
      		        count+=(bitwiseCompare(binaryRightShift(mask,2),1))
        		    input(input(3+it+numTh).asInstanceOf[Rep[Int]]+varIntToRepInt(count)+displacementVal)=varIntToRepInt(basetidVariable).asInstanceOf[Rep[Float]]
    		        basetidVariable+=1
    		        count+=(bitwiseCompare(binaryRightShift(mask,3),1))
    		     }
    	         else{
    	           if((bitwiseCompare(mask,1)).asInstanceOf[Rep[Boolean]]){
    	             input(input(3+it+numTh).asInstanceOf[Rep[Int]]+varIntToRepInt(count)+displacementVal)=varIntToRepInt(basetidVariable).asInstanceOf[Rep[Float]]
    		         count+=1
    	           }
    	           basetidVariable+=1
    	           if((bitwiseCompare(binaryRightShift(mask,1),1)).asInstanceOf[Rep[Boolean]]){
    	             input(input(3+it+numTh).asInstanceOf[Rep[Int]]+varIntToRepInt(count)+displacementVal)=varIntToRepInt(basetidVariable).asInstanceOf[Rep[Float]]
    		         count+=1
    		       }
    	           basetidVariable+=1
    	           if((bitwiseCompare(binaryRightShift(mask,2),1)).asInstanceOf[Rep[Boolean]]){
    	             input(input(3+it+numTh).asInstanceOf[Rep[Int]]+varIntToRepInt(count)+displacementVal)=varIntToRepInt(basetidVariable).asInstanceOf[Rep[Float]]
    		         count+=1
    		       }
    	           basetidVariable+=1
    	           if((bitwiseCompare(binaryRightShift(mask,3),1)).asInstanceOf[Rep[Boolean]]){
    	             input(input(3+it+numTh).asInstanceOf[Rep[Int]]+varIntToRepInt(count)+displacementVal)=varIntToRepInt(basetidVariable).asInstanceOf[Rep[Float]]
    		         count+=1
    		       }
    	           basetidVariable+=1
    	         }
      	         currInst+=1
    	       }
    		}  
    	  }
    	    
    	  override def innerRunLoop(numIn:Int, numItU:Rep[Int], iteration:(Rep[Int],(Rep[Int],Rep[Int])=>Unit, Rep[Int])=>Unit, instOfIt:(Rep[Int],Rep[Int])=>Unit, dispIt:Rep[Int], isbfc:Boolean, isparallel:Boolean, numTh:Rep[Int]): Var[Int] = {
    	      val sizeoffloat:Rep[Int]=sizeof("float")
    	      val sizeofm128:Rep[Int]=sizeof("__m128")
    	      var sse_array: Rep[Array[Float]]=(referenceOperator(input(dispIt))).asInstanceOf[Rep[Array[Float]]]
    	      var alignment_offset: Rep[Int]=sizeofm128-((intptr_t(sse_array))%sizeofm128)
    	      var basetid: Rep[Int]= (alignment_offset/sizeoffloat) //Add result_tid_offset for threads
    	      var sse_array_length: Rep[Int]= (numIterations-basetid)*sizeoffloat/sizeofm128
    	      var tmp_array: Rep[Array[Char]]= sse_array.asInstanceOf[Rep[Array[Char]]]
    	      sse_array=tmp_array.asInstanceOf[Rep[Array[Int]]]
    	      var maxIterationCovered:Variable[Int]=0+basetid
    	      //Alignment loop
    	      if (alignment_offset!=0){
    	        for (i<-(0 until basetid): Rep[Range]){
    	          instOfIt(i,dispIt)
    	    	}
    	      }
              //Vectorized loop
    	      if(isparallel){
    	        if (basetid+((4*numIn*numTh))<numIterations){
    	          //Parallel prefix sum...
    	          for (j <- (0 until numTh): Rep[Range]) {
    	            runVectorizedParallelPrefixSum(j, numIn, numItU, isbfc, numTh, sse_array_length, sse_array)		
    		      } 
                  //Serial assignment of output positions...
    	          var tempVal:Rep[Int]=3+numTh
    	    	  input(tempVal)=varIntToRepInt(outputPos).asInstanceOf[Rep[Float]]
    	    	  for (k <- (1 until numTh): Rep[Range]) {
    	    	    input(tempVal+k)=input(3+k-1)+input(tempVal+k-1)
    	    	  }
    	          outputPos=(input(tempVal-1)+input(tempVal-1+numTh)).asInstanceOf[Rep[Int]]
    	          //Parallel writing...
    	          for (j <- (0 until numTh): Rep[Range]) {
    	            runVectorizedParallelWriting(j, numIn, numItU, isbfc, (basetid+(j*numTh*sse_array_length/(numIn*numTh))), numTh, dispIt, sse_array_length, sse_array)
    		      }
    	          if(((sse_array_length.asInstanceOf[Rep[Double]]/(numIn*numTh).asInstanceOf[Rep[Double]]))!=((sse_array_length/(numIn*numTh)))){
    	            maxIterationCovered+=(sse_array_length*numIn*numTh)//Cases where the array cannot be exactly divided among threads.
    	          }
    	          else{
    	        	maxIterationCovered+=(sse_array_length*sizeofm128/sizeoffloat) //Cases where the array is exactly divided among threads
    	          }
    	        }
    	      }
    	      else{
    	        var basetidToWrite: Var[Int]= 0 +basetid
    	        var comp_val: Rep[Int]= _mm_set1_ps(value)
     	        var read_val: Rep[Int]= _mm_set1_ps(zero.asInstanceOf[Rep[Float]])
      	        var comp_result: Rep[Int]= _mm_set1_ps(zero.asInstanceOf[Rep[Float]])
    	        val displacementVal: Rep[Int]= dispIt+numIterations
    	        for (i<- (0 until sse_array_length/numIn): Rep[Range]){
      	          var currInst:Int=0
      	          while (currInst<numIn){
      	            read_val=_mm_load_ps(referenceOperator(sse_array((numIn*i)+currInst).asInstanceOf[Rep[Float]]).asInstanceOf[Rep[Array[Float]]])
      	            comp_result=evalVect(read_val, comp_val)//Vectorized comparison
    		        var mask: Rep[Int]= _mm_movemask_ps(comp_result)
    		        vectorizedRunInstructionOfIteration(varIntToRepInt(basetidToWrite), displacementVal, mask, isbfc)
    		        basetidToWrite+=1 
    		        vectorizedRunInstructionOfIteration(varIntToRepInt(basetidToWrite), displacementVal, binaryRightShift(mask,1), isbfc)
    		        basetidToWrite+=1
    		        vectorizedRunInstructionOfIteration(varIntToRepInt(basetidToWrite), displacementVal, binaryRightShift(mask,2), isbfc)
    		        basetidToWrite+=1
    		        vectorizedRunInstructionOfIteration(varIntToRepInt(basetidToWrite), displacementVal, binaryRightShift(mask,3), isbfc)
    		        basetidToWrite+=1
      	            currInst+=1
    		      } 
                }
    	        maxIterationCovered+=(sse_array_length*sizeofm128/sizeoffloat)
    	      }
    	      //Residual iterations
    	      for(i <-(varIntToRepInt(maxIterationCovered) until numIterations): Rep[Range]){ 
                  instOfIt(i,dispIt)
              }
    	      outputPos
    	  }
    			
    	  override def runLoop(): Var[Int]= {
    	    if (decoratedLoop.vectorized){ //Do nothing
    	      decoratedLoop.innerRunLoop(numInst, numIterationsUnrolled, runIteration, runInstructionOfIteration, displacementForIterationPos, bfc, parallel, numThreads)
    	    }
    	    else {
    	      innerRunLoop(numInst, numIterationsUnrolled, runIteration, runInstructionOfIteration, displacementForIterationPos, bfc, parallel, numThreads)    		        	        	    
    	    }  	
    	  }
    		  
    	}
    		
       	/*Class implementing GPU parallelization*/
    	class CudaLoop (decoratedLoop: SimpleLoop) extends LoopDecorator (decoratedLoop) {
    	  
    	  /*Note: Overriden functions for CudaLoop at this point are empty, and we just include them as a working template*/
    	  override def runInstructionOfIteration(it: Rep[Int], dispIt:Rep[Int])= comment("decorated instruction", verbose = true){
    	    decoratedLoop.runInstructionOfIteration(it, dispIt)
    	  }
    			
    	  override def runIteration(it: Rep[Int], instOfIt:(Rep[Int],Rep[Int])=>Unit, dispIt:Rep[Int])= comment("decorated iteration", verbose = true){
    		decoratedLoop.runIteration(it, runInstructionOfIteration, dispIt)
    	  }
    	  
    	  override def innerRunLoop(numIn:Int, numItU:Rep[Int], iteration:(Rep[Int],(Rep[Int],Rep[Int])=>Unit, Rep[Int])=>Unit, instOfIt:(Rep[Int],Rep[Int])=>Unit, dispIt:Rep[Int], isbfc: Boolean, isparallel: Boolean, numTh: Rep[Int]): Var[Int] = {
    	    decoratedLoop.innerRunLoop(numIn, numItU, runIteration, runInstructionOfIteration, displacementForIterationPos, decoratedLoop.bfc, isparallel, numTh)
    	  }
    			
    	  override def runLoop(): Var[Int]={
    	    innerRunLoop(decoratedLoop.numInst, decoratedLoop.numIterationsUnrolled, runIteration, runInstructionOfIteration, displacementForIterationPos, bfc, parallel, numThreads)
    	  }  	
    		
    	}

    /***********************************************************************************************************
   	2. PROCESSING SECTION- Crucial area of the library, where the code is generated
    ***********************************************************************************************************/
    		
    	/*Creation of the generic loop*/       
    	var loop: SimpleLoop = new SimpleLoop(valueForComparison, maxNumIt, predicateAssigned)
    	
    	/*Variables for configuration (apart from instructions array)*/
    	val unrollDepthRequested:Int=4
    	val vectorSizeRequested:Int=4 //Only 4 is supported in this version.

    	/*Application of variants to the generic loop.*/
    	for (instruction<-instructions){	  
    		instruction match {
    		  case "Remove branching" => loop = new BranchFreeLoop(loop)
    		  case "Unroll" => loop = new UnrolledLoop(loop, unrollDepthRequested)
    		  case "Parallelize" => loop = new ParallelLoop(loop)
    		  case "Vectorize" => loop = new VectorizedLoop(loop, vectorSizeRequested)
    		  case "GPU" => loop = new CudaLoop(loop)
    		  case _ =>{} //Do nothing... Added to keep with format.
    		}
    	}
  		
    	/*Now that the variant loop has been configured we can generate it's code...*/
    		
    	/*Code generation for the loop.*/
    	var nhits: Rep[Int]=varIntToRepInt(loop.runLoop()) 
		    
    	/*Printing of the output array*/
    	println("Number of tuples found: ")	
    	println(nhits)
    	if(nhits!=zero){
    		println("Output array: ")
    		var baseValue:Rep[Int]= loop.displacementForIterationPos+loop.numIterations
    		for (i <- (0 until nhits): Rep[Range]) {
    			println(input(i+baseValue))
    		}
    	}
    			
      }//End of snippet function
      
      /*End of area where context is shared between 2 stages*/
   
    }//End of use of DSL driver
    
    /*Only Scala code now...*/
    
    check("ScanVariants_"+predicateAssigned, snippet.code, "c")     /*The naming scheme can be modified in this line*/
    
    /***********************************************************************************************************
    3. POST-PROCESSING SECTION- Can be considered to be additional to the base program. 
    In future versions, this section could be removed altogether.
    ***********************************************************************************************************/
    
    /*Post-processing of the generated file so as to include Parallelization and Vectorization*/
    var fileLines = io.Source.fromFile("src/out/c_code_generation_tests_ScanVariants_"+predicateAssigned+".check.c").getLines.toList
    
    if (instructions.contains("Vectorize")){//For vectorization we need to add __m128 types, pointer handling and some castings
      if(fileLines.filter(x=>x.contains("&(")).length>0){ 
        /**The next bit fixes code that is generated like this:
         *  float x180 = x8[x179];
         *  float* x181 = &(x180);
         *  
         *  So it looks like this:
         *  float* x181 = (float*) &(x8[x179]);
         *  
         *  The first case is of type __m128. 
         *  The portion that is done in the while loop is for the unrolled case.
         * */
        var displacementLine= fileLines.filter(x=>x.contains("&(")).head
        var displacementLineSplit = displacementLine.split("&")
        var firstVariableName=displacementLineSplit(1).replace("(", "").replace(")", "").replace(";","")
        var substitutionLine=fileLines.filter(x=>x.contains(firstVariableName+" = ")).head
        var substitutionLineSplitted=substitutionLine.split("=")
        var substitutionString=substitutionLineSplitted(1).replace(";", "").replace(" ", "")
        fileLines=fileLines diff List(substitutionLine)
        fileLines=fileLines.updated(fileLines.indexOf(displacementLine), displacementLine.replace(firstVariableName, substitutionString).replace("float*", "__m128*").replace("=", "= (__m128*)"))  
        var sseArrayList= fileLines.filter(x=>x.contains("&(")).tail
        while (!sseArrayList.isEmpty){ //Dealing with unrolling
          var sseArrayLine = sseArrayList.head
          var sseArrayLineSplitted = sseArrayLine.split("&")
          var secondVariableName=sseArrayLineSplitted(1).replace("(", "").replace(")", "").replace(";","")
          var substitutionLine2=fileLines.filter(x=>x.contains(secondVariableName+" = ")).head
          var substitutionLine2Splitted= substitutionLine2.split("=")
          var substitutionString2=substitutionLine2Splitted(1).replace(";", "").replace(" ", "")
          fileLines=fileLines diff List(substitutionLine2)
          fileLines=fileLines.updated(fileLines.indexOf(sseArrayLine), sseArrayLine.replace(secondVariableName, substitutionString2).replace("=", "= (float*)"))
          sseArrayList= sseArrayList diff List(sseArrayLine)
        }
        /**The following section is in charge of inserting lines like this:
         *   char* tmp_array= (char*) x8;
         *   tmp_array+=x11;
         *   x8=(__m128*)(tmp_array);
         *   */
        displacementLineSplit= displacementLine.split("=")
        var arrayVariable= displacementLineSplit(0).replace("float*", "").replace(" ", "")
        def insert[A](xs: List[A], extra: List[A])(p: A => Boolean) = {
        	xs.map(x => if (p(x)) extra ::: List(x) else List(x)).flatten
        }
        var moduloVariableSplitted=fileLines.filter(x=>x.contains("%")).head.replace(" ","").replace("int32_t", "").split("=")
        var moduloVariable=moduloVariableSplitted(0)
        var alignmentOffsetVariableSplitted=fileLines.filter(x=>x.contains("- "+moduloVariable+";")).head.replace("int32_t", "").split("=")
        var alignmentOffsetVariable=alignmentOffsetVariableSplitted(0).replace(" ","")
        fileLines=insert(fileLines,List("  char* tmp_array= (char*) "+arrayVariable+";","  tmp_array+="+alignmentOffsetVariable+";", "  "+arrayVariable+"=(__m128*)(tmp_array);")){_ == fileLines.filter(x=>x.contains(alignmentOffsetVariable+" == 0;")).head}
        /**The following bit changes type of variables asssinged from functions starting with:"_mm*", so they are of type __m128.
         * The exception is the function_mm_movemask_ps, whose return type is int32_t. */
        var mmsetFileLines= Vector(fileLines.filter(x=>x.contains("_mm_")):_*)
        for (mmsetFileLine <-mmsetFileLines){
          if (mmsetFileLine.contains("_mm_movemask_ps")){
            var altString=mmsetFileLine.replace("_mm_movemask_ps(","_mm_movemask_ps((__m128)")
            fileLines=fileLines.updated(fileLines.indexOf(mmsetFileLine), altString)
          }
          else{
            var altString=mmsetFileLine.replace("int32_t","__m128")
            fileLines=fileLines.updated(fileLines.indexOf(mmsetFileLine), altString)
          }
        }  
        /*We write back to the file*/	
	    val pw = new PrintWriter(new File("src/out/c_code_generation_tests_ScanVariants_"+predicateAssigned+".check.c"))
	    for (str<- fileLines){
	    	pw.write(str+"\n")
	    }
	    pw.close()
      }
    }
     
     /**We check if the code was parallelized, by seeing if it has as input the number of threads and if it has loops that
     * use this number as limit for the iteration space (this is a characteristic exclusive to code that uses parallelization).
     * Optionally the instructions array could also be used. */
    if(instructions.contains("Parallelize") && fileLines.filter(x=>x.contains("x0[2]")).length>0){ /**For parallelization we need to add the pthread library instructions and 
     																								* re-order the code accordingly*/
    	val numThreadsLine=fileLines.filter(x=>x.contains("x0[2]")).head
    	var arrayOfSplitting: Array[java.lang.String] = numThreadsLine.split(" ")
   	   	val numThreadsIdentifier=arrayOfSplitting(3)
   	   	val loopHeaders= fileLines.filter(x=> x.contains("for(")).filter(x=> x.contains("< "+numThreadsIdentifier+";"))
   	   	if (loopHeaders.length>0){
   	   		val prefixSumHeader=loopHeaders(0)
   	   		val parallelWritingHeader=loopHeaders(2)
   	   		arrayOfSplitting= prefixSumHeader.split("=")
   	   		arrayOfSplitting=arrayOfSplitting(0).replace("for(","").replace(" ","").split("x")
   	   		val integerRepresentation= arrayOfSplitting(0)
   	   		val iteratorName_prefixSum="x"+arrayOfSplitting(1)
   	   		arrayOfSplitting = parallelWritingHeader.split("=")
   	   		arrayOfSplitting = arrayOfSplitting(0).replace("for(","").replace(" ","").split("x")
   	   		val iteratorName_parallelWriting="x"+arrayOfSplitting(1)	
   	   		def insert[A](xs: List[A], extra: List[A])(p: A => Boolean) = {
   	   			xs.map(x => if (p(x)) extra ::: List(x) else List(x)).flatten
   	   		}
   	   		var auxList  = List[String]()
   	   		auxList= "  pthread_t threads[("+integerRepresentation+")"+numThreadsIdentifier+"];"::auxList
   	   		auxList=auxList:+("  "+integerRepresentation+" *inputArray;")
	        auxList=auxList:+("  inputArray=("+integerRepresentation+"*)malloc("+numThreadsIdentifier+"*sizeof("+integerRepresentation+"));")
	        fileLines=insert(fileLines,auxList) {_ == prefixSumHeader}
   	   		var str: String =" ";
	        var forHeaderDetected: Boolean = false
	        var firstMessageSpotted: Boolean = false
	        var doNothing: Boolean = false
	        var scanDetected: Boolean = false
	        auxList  = List[String]()
	        var outputList  = List[String]()
	        for (str<- fileLines){
	        	if (!scanDetected){
	        		if (str.contains("*****************************************")){
	        			outputList=outputList:+("#include <fcntl.h>")
	        			outputList=outputList:+("#include <errno.h>")
	        			outputList=outputList:+("#include <err.h>")
	        			outputList=outputList:+("#include <sys/mman.h>")
	        			outputList=outputList:+("#include <sys/stat.h>")
	        			outputList=outputList:+("#include <stdio.h>")
	        			outputList=outputList:+("#include <stdint.h>")
	        			outputList=outputList:+("#include <unistd.h>")
	        			outputList=outputList:+("#include <stdlib.h>")
	        			outputList=outputList:+("#include <pthread.h>")
	        			outputList=outputList:+("#include <emmintrin.h>")
	        			outputList=outputList:+("#include <mmintrin.h>")
	        			outputList=outputList:+("void Scan(float*);")
	        			outputList=outputList:+("int main(int argc, char *argv[])")
	        			outputList=outputList:+("{")
	        			outputList=outputList:+("  if (argc < 5) {")
	        			outputList=outputList:+("    printf(\"Missing arguments. Usage: filename numberOfTuples compareValue numThreads\\n\");")
	        			outputList=outputList:+("    return 0;")
	        			outputList=outputList:+("  }")
	        			outputList=outputList:+("  printf(\"Usage: filename numberOfTuples compareValue numThreads\\n\");")
	        			outputList=outputList:+("  FILE *ptr_file;")
	        			outputList=outputList:+("  char buf[1000];")
	        			outputList=outputList:+("  int numTuples=atoi(argv[2]);")
	        			outputList=outputList:+("  float compareValue=atof(argv[3]);")
	        			outputList=outputList:+("  int numThreads=atoi(argv[4]);")
	        			outputList=outputList:+("  int numReadTuples=0;")
	        			outputList=outputList:+("  ptr_file =fopen(argv[1],\"r\");")
	        			outputList=outputList:+("  if (!ptr_file){")
	          			outputList=outputList:+("    printf(\"Error. Could not open the input file.\\n\");")
	        			outputList=outputList:+("    return 0;")
	        			outputList=outputList:+("  }")    
	        			outputList=outputList:+("  if (numTuples<=0){")
	        			outputList=outputList:+("    printf(\"Error. Please pass a valid number of tuples.\\n\");")
	        			outputList=outputList:+("    return 0;")
	        			outputList=outputList:+("  }")
	        			outputList=outputList:+("  if (numThreads<=0){")
	        			outputList=outputList:+("    printf(\"Error. Please pass a valid number of threads.\\n\");")
	        			outputList=outputList:+("    return 0;")
	        			outputList=outputList:+("  }")
	        			outputList=outputList:+("  float *array;")
	        			outputList=outputList:+("  array=(float*)malloc(((2*numTuples)+3+(2*numThreads))*sizeof(float));")
	        			outputList=outputList:+("  array[0]=compareValue;")
	        			outputList=outputList:+("  array[1]=(float)numTuples;")
	        			outputList=outputList:+("  array[2]=(float)numThreads;")
	        			outputList=outputList:+("  for (int i=0; i<(2*numThreads); i++){")
	        			outputList=outputList:+("    array[3+i]=(float)0;")
	        			outputList=outputList:+("  }")
	        			outputList=outputList:+("  while (fgets(buf,1000, ptr_file)!=NULL && numReadTuples<numTuples){")
	        			outputList=outputList:+("    array[numReadTuples+3+(2*numThreads)]=atof(buf);")
	        			outputList=outputList:+("    numReadTuples++;")
	        			outputList=outputList:+("  }")
	        			outputList=outputList:+("  fclose(ptr_file);")
	        			outputList=outputList:+("  if (numReadTuples<numTuples){")
	        			outputList=outputList:+("    printf(\"Error, file contains less tuples than specified.\\n\");");
	        			outputList=outputList:+("    return 0;")
	        			outputList=outputList:+("  }")
	        			outputList=outputList:+("  Scan(array);")
	        			outputList=outputList:+("  return 1;")
	        			outputList=outputList:+("}")
	        			outputList=outputList:+("/*****************************************")
	        			scanDetected=true
	        		}
	        		else{
	        			//Do nothing...
	        		}
	        	}
	        	else if (!forHeaderDetected){
	        		if (str==prefixSumHeader){
	        			forHeaderDetected=true
	        		}
	        		else{
	        			outputList=outputList:+(str)
	        		}
	        	}
	        	else{
	        		if (!firstMessageSpotted){
	        			auxList=auxList:+(str)
	        			if(str.contains("//#parallel prefix sum")){
	        				firstMessageSpotted=true
	        			}
	        		}
	        		else if (!doNothing){
	        			auxList=auxList:+(str)
	        			if(str.contains("//#parallel prefix sum")){
	        				doNothing=true
	        			}
	        		}
	        	}
	        }
	        outputList=outputList:+("  void* parallelPrefixSum(void* input){")
	        outputList=outputList:+("    "+integerRepresentation+" "+iteratorName_prefixSum+"=*("+integerRepresentation+"*)input;")
	        outputList=outputList++auxList
	        outputList=outputList:+("  }")
	        outputList=outputList:+(prefixSumHeader)
	        outputList=outputList:+("	inputArray["+iteratorName_prefixSum+"]="+iteratorName_prefixSum+";")
	        outputList=outputList:+("	pthread_create(&threads["+iteratorName_prefixSum+"], NULL, parallelPrefixSum, (void *)&inputArray["+iteratorName_prefixSum+"]);")
	        outputList=outputList:+("  }")
	        outputList=outputList:+(prefixSumHeader)
	        outputList=outputList:+("	pthread_join(threads["+iteratorName_prefixSum+"], NULL);")
	        outputList=outputList:+("  }")
	        auxList  = List[String]()
	        forHeaderDetected= false
	        firstMessageSpotted= false
	        var secondMessageSpotted:Boolean= false
	        var thirdMessageSpotted:Boolean= false
	        var forthMessageSpotted:Boolean= false
	        var trailingLineRemoved:Boolean= false
	        doNothing= false
	        for (str<- fileLines){
	        	if (!firstMessageSpotted){
	        		if(str.contains("//#parallel prefix sum")){
	        			firstMessageSpotted=true
	        		}
	        	}
	        	else if (!secondMessageSpotted){
	        		if(str.contains("//#parallel prefix sum")){
	        			secondMessageSpotted=true
	        		}
	        	}
	        	else if(!trailingLineRemoved){
	        		trailingLineRemoved=true
	        	}
	        	else if (!forHeaderDetected){
	        		if(str!=parallelWritingHeader){
	        			outputList=outputList:+(str)
	        		}
	        		else{
	        			forHeaderDetected=true
	        		}
	        	}
	        	else if (!thirdMessageSpotted){
	        		auxList=auxList:+(str)
	        		if(str.contains("//#parallel writing")){
	        			thirdMessageSpotted=true
	        		}			
	        	}	
	        	else if (!forthMessageSpotted){
	        		auxList=auxList:+(str)
	        		if(str.contains("//#parallel writing")){
	        			forthMessageSpotted=true
	        		}			
	        	}			
	        }
	        outputList=outputList:+("  void* parallelWriting(void* input){")
	        outputList=outputList:+("    "+integerRepresentation+" "+iteratorName_parallelWriting+"=*("+integerRepresentation+"*)input;")
	        outputList=outputList++auxList
	        outputList=outputList:+("  }")
	        outputList=outputList:+(parallelWritingHeader)
	        outputList=outputList:+("  	pthread_create(&threads["+iteratorName_parallelWriting+"], NULL, parallelWriting, (void *)&inputArray["+iteratorName_parallelWriting+"]);") 
	        outputList=outputList:+("  }")
	        outputList=outputList:+(parallelWritingHeader)
	        outputList=outputList:+("	pthread_join(threads["+iteratorName_parallelWriting+"], NULL);")
	        outputList=outputList:+("  }")
	        firstMessageSpotted=false
	        secondMessageSpotted=false
	        trailingLineRemoved= false
	        for (str <-fileLines){
	        	if(!firstMessageSpotted){
	        		if(str.contains("//#parallel writing")){
	        			firstMessageSpotted=true
	        		}
	        	}
	        	else if (!secondMessageSpotted){
	        		if(str.contains("//#parallel writing")){
	        			secondMessageSpotted=true
	        		}
	        	}
	        	else if (!trailingLineRemoved){
	        		trailingLineRemoved=true
	        	}
	        	else{
	        		outputList=outputList:+(str)
	        	}
	        }
	        /*We write back to the file*/	
	        val pw = new PrintWriter(new File("src/out/c_code_generation_tests_ScanVariants_"+predicateAssigned+".check.c"))
	        for (str<- outputList){
	        	pw.write(str+"\n")
	        }
	        pw.close()
   	   	}//End of if loopHeaders.length>0
    }//End of if  fileLines.filter(x=>x.contains("x0[2]")).length>0 
    
  } //End of test "Scan Variants"
} //End of class ScanVariantsTest
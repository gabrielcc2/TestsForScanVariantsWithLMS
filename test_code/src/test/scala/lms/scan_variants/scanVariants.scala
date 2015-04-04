/*******************************************************************************************************************************************
ScalaVariantTests class, defining some initial tests for developing a code-generating scheme for scan variants,
based on EPFL's Light-weight modular staging library and a slightly modified version of their DSL api for 
C-code generation.

Author: 
Gabriel Campero
gabrielcampero@acm.org

Used library based on LMS source code by Tiark Rompf and others, retrieved on December, 2014, from: http://scala-lms.github.io/
References used: 
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
decide which former functions to inherit or override. 

	BRANCH REMOVAL: Only one call of remove branching is effectful. More calls dont add anything new to the execution.

	LOOP UNROLLING: Loop unrolling can be added up. If there is an attempt to use an unrolled loop beyond, but passing less tuples
    than there are instructions in one unrolled iteraton, then the whole loop will run regularly in a section defined as the residue of the unrolling. 

	THREAD-LEVEL PARALLELIZATION: As it is implemented, only one parallelization is defined, dividing the iterations among them. 
	Number of threads will be passed as parameter at run time.
	When parallelizing, unrolling is always performed inside the parallel sections.
	Adding more threads than there are iterations will have no effect, and the loop will run unparallelized in the section defined as the residue
    of the unrolling.
	
	VECTORIZATION: Only a vector of size 4 is supported in this version.
	
	GPU PARALLELIZATION:

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
    	     * Check extensions to branch vs. non-branch, in different orders
    	     * Deal with unrolling
    	     * Deal with parallelization
    	     * Combinatory testing with input + Generate Code Examples    	
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
    
    var instructions = new Array[String](numVariantsToApply) 
    instructions(0) = "_"//"Unroll"
    instructions(1) = "_"//"Remove branching"
    instructions(2) = "Parallelize"//"Parallelize"
    instructions(3) = "_"//"Vectorize"//"Vectorize"
   
    /**Configuration for the predicate
     * Could be EQUAL, GREATER, GREATER_EQUAL, LESSER, LESSER_EQUAL, 
     * NOT_EQUAL. 
     * Anything else would mean scan all.**/    
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
    			
    	  def displacementForIterationPos: Rep[Int] /*Displacement for iteration position, 
    	   											* Allowing to map between logical iteration number
    	   											* and its position in the input array, following the
    	   											* our scheme. */ 
    			
    	  /*Flags*/
    			
    	  def bfc: Boolean //Flag indicating if branch free code was selected
    			
    	  def parallel: Boolean //Flag indicating if parallelization has been selected
    	  
       	  def vectorized: Boolean //Flag indicating if vectorization has been selected

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
    			    						
    	  /**runInstructionOfIteration: Defines one single step of an iteration (w.r.t. the original iterations)*/
    	  def runInstructionOfIteration(it: Rep[Int], dispIt: Rep[Int])= comment("run instruction with branching", verbose = true){
    	    var itVal:Rep[Int]=it+dispIt
    	    if (eval(input(itVal),value.asInstanceOf[Rep[Float]])){ //Branching code
    	      input(varIntToRepInt(outputPos)+dispIt+numIterations)=(it).asInstanceOf[Rep[Float]]
    	      outputPos+=1
    		}
    	  }
 
    	  /**runIteration: All the steps to be performed during an interation*/
    	  def runIteration(it: Rep[Int], instOfIt:(Rep[Int],Rep[Int])=>Unit)= comment("run iteration as in simple loop", verbose = true){
    	    instOfIt(it, displacementForIterationPos)
    	  }
    	  
    	   /** innerRunLoop: Inner function responsible for running the loop (in this case, a simple loop) 
    	   *  Returns the number of hits found.
    	   *  Takes as input the function in charge of the iteration, and the function in charge of the instruction per iteration*/
    	  def innerRunLoop(numIn:Int, numItU:Rep[Int], iteration:(Rep[Int],(Rep[Int], Rep[Int])=>Unit)=>Unit, instOfIt:(Rep[Int],Rep[Int])=>Unit, isbfc: Boolean): Var[Int] = {
    	    /*Loop for code generation*/
    		for (i <- (0 until numIterations): Rep[Range]) {
    		  iteration(i, instOfIt)
    		}
    		outputPos
    	  }
    			    			
    	  /** runLoop: Outer function calling the execution of the loop (in this case, a simple loop)  
    	   *  Returns the number of hits found.*/
    	  def runLoop(): Var[Int] = {
    	    innerRunLoop(numInst, numIterationsUnrolled, runIteration, runInstructionOfIteration, bfc)
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
    	  
    	  override def runIteration(it: Rep[Int], instOfIt:(Rep[Int],Rep[Int])=>Unit)= comment("decorated iteration", verbose = true){
    		decoratedLoop.runIteration(it, instOfIt)
    	  }
    	  
    	  override def innerRunLoop(numIn:Int, numItU:Rep[Int], iteration:(Rep[Int],(Rep[Int],Rep[Int])=>Unit)=>Unit, instOfIt:(Rep[Int],Rep[Int])=>Unit, isbfc: Boolean): Var[Int] = {
    	    decoratedLoop.innerRunLoop(numIn, numItU, iteration, instOfIt, isbfc)
    	  }
    			
    	  override def runLoop(): Var[Int]={
    	    innerRunLoop(decoratedLoop.numInst, decoratedLoop.numIterationsUnrolled, runIteration, runInstructionOfIteration, decoratedLoop.bfc)
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
    	  
    	  override def runLoop(): Var[Int]={
    	    innerRunLoop(decoratedLoop.numInst, decoratedLoop.numIterationsUnrolled, runIteration, runInstructionOfIteration, bfc)
    	  }
    		  
    	}
    		
    	/*Class implementing loop unrolling*/
    	class UnrolledLoop (decoratedLoop: SimpleLoop, unrollDepth:Int) extends LoopDecorator(decoratedLoop){
    		  
    	  override def numIterationsUnrolled=((decoratedLoop.numIterationsUnrolled).asInstanceOf[Rep[Float]]/unrollDepth).asInstanceOf[Rep[Int]]
   			  
    	  override def numInst=(decoratedLoop.numInst*(unrollDepth.asInstanceOf[Int]))
  
    	  override def runIteration(it: Rep[Int], instOfIt:(Rep[Int],Rep[Int])=>Unit)= comment("run iteration from unrolled loop", verbose = true){
    		var currInst:Int=0
    		val baseIt:Rep[Int]=(it*numInst)
    		while (currInst<numInst){
    		  var itVal:Rep[Int]=currInst+baseIt
    		  instOfIt(itVal, decoratedLoop.displacementForIterationPos)
    		  currInst+=1
    		} 
    	  }
    		  
    	  override def innerRunLoop(numIn:Int, numItU:Rep[Int], iteration:(Rep[Int],(Rep[Int], Rep[Int])=>Unit)=>Unit, instOfIt:(Rep[Int], Rep[Int])=>Unit, isbfc: Boolean): Var[Int] = {
    	    if (decoratedLoop.parallel){
    	      decoratedLoop.innerRunLoop(numIn, numItU, iteration, instOfIt, isbfc)
    	    }
    		else{
    			/*Loop for code generation*/
    			for (i <- (0 until numItU): Rep[Range]) {
    			  iteration(i, instOfIt)
    		    }
    			/*Residual iterations*/
    			val maxIterationCovered:Rep[Int]=numItU*numIn
    			if(maxIterationCovered<numIterations){
    			  for (i <- (maxIterationCovered until numIterations): Rep[Range]) {
    			    instOfIt(i, decoratedLoop.displacementForIterationPos)
    			  }
    			}
    		    outputPos
    		}
    	  }

    	  override def runLoop(): Var[Int] = {
    	    innerRunLoop(numInst, numIterationsUnrolled, runIteration, decoratedLoop.runInstructionOfIteration, decoratedLoop.bfc)
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

    	  override def runInstructionOfIteration(it: Rep[Int], dispIt:Rep[Int])= comment("decorated instruction", verbose = true){
    		  decoratedLoop.runInstructionOfIteration(it, dispIt)
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
    			itVal+=((i+((numItU/numThreads).asInstanceOf[Rep[Int]]*it))*numInst)
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
    			
    	  override def innerRunLoop(numIn:Int, numItUn:Rep[Int], iteration:(Rep[Int],(Rep[Int], Rep[Int])=>Unit)=>Unit, instOfIt:(Rep[Int],Rep[Int])=>Unit, isbfc: Boolean): Var[Int] = {
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
    		  outputPos=input(tempVal-1)+input(tempVal-1+numThreads)
    		}
    				  
    		//Parallel writing...					
    		for (l <- (0 until numThreads): Rep[Range]) {
    		  this.runParallelWriting(l, numIn, numItUn, isbfc)
    		}
    
    		//Residual iterations
    		var maxIterationCovered:Rep[Int]= (numItUn/numThreads)*numIn*numThreads.asInstanceOf[Rep[Int]]
    		if(maxIterationCovered<numIterations){
    		  for (i <- (maxIterationCovered until numIterations): Rep[Range]) {
    		    instOfIt(i, displacementForIterationPos)
    		  }
    		}
    		outputPos
    	  }
    			    			    			    			    			
    	}
    		
    	/*Class implementing vectorization*/
    	class VectorizedLoop (decoratedLoop: SimpleLoop, vectorSize: Int) extends LoopDecorator (decoratedLoop) {
    	  
    	  override def vectorized=true
    	  
    	  /*Note: Overriden functions at this point are still empty, and are just included as a working template.
    	   * Except for runLoop.*/
    	  override def runInstructionOfIteration(it: Rep[Int], dispIt:Rep[Int])= comment("decorated instruction", verbose = true){
    	    decoratedLoop.runInstructionOfIteration(it, dispIt)
    	  }
    		  	
    	  override def runIteration(it: Rep[Int], instOfIt:(Rep[Int],Rep[Int])=>Unit)= comment("decorated iteration", verbose = true){
    		decoratedLoop.runIteration(it, runInstructionOfIteration)
    	  }
    	  
    	  override def innerRunLoop(numIn:Int, numItU:Rep[Int], iteration:(Rep[Int],(Rep[Int],Rep[Int])=>Unit)=>Unit, instOfIt:(Rep[Int],Rep[Int])=>Unit, isbfc:Boolean): Var[Int] = {
    	    for (i <- (0 until maxNumIt): Rep[Range]) {
    		  iteration(i, instOfIt)
    		}
    	    outputPos
    	  }
    			
    	  override def runLoop(): Var[Int]= {
    	    
    	    if (decoratedLoop.vectorized){ //Do nothing
    	      innerRunLoop(numInst, numIterationsUnrolled, runIteration, runInstructionOfIteration, bfc)
    	    }
    	    else {
    	      val sizeoffloat:Rep[Int]=sizeof("float")
    	      val sizeofm128:Rep[Int]=sizeof("__m128")
    	      var sse_array: Rep[Array[Float]]=(referenceOperator(input(displacementForIterationPos))).asInstanceOf[Rep[Array[Float]]]
    	      var alignment_offset: Rep[Int]=sizeofm128-((intptr_t(sse_array))%sizeofm128)
    	      var basetid: Rep[Int]= (alignment_offset/sizeoffloat) //Add result_tid_offset for threads
    	      var sse_array_length: Rep[Int]= (maxNumIt-basetid)*sizeoffloat/sizeofm128
    	      var tmp_array: Rep[Array[Char]]= sse_array.asInstanceOf[Rep[Array[Char]]]
    	      sse_array=tmp_array.asInstanceOf[Rep[Array[Int]]]
    	      if (alignment_offset!=0){
    	    	  for (i<-(0 until basetid): Rep[Range]){
    	    	    runInstructionOfIteration(i, displacementForIterationPos)
    	    	  }
    	      }
    	      var basetidToWrite: Var[Int]= 0 +basetid
    	      var comp_val: Rep[Int]= _mm_set1_ps(value)
     	      var read_val: Rep[Int]= _mm_set1_ps(zero.asInstanceOf[Rep[Float]])
      	      var comp_result: Rep[Int]= _mm_set1_ps(zero.asInstanceOf[Rep[Float]])
      	      val displacementVal: Rep[Int]= displacementForIterationPos+maxNumIt
      	      for (i<- (0 until sse_array_length): Rep[Range]){
      	        read_val=_mm_load_ps(referenceOperator(sse_array(i).asInstanceOf[Rep[Float]]).asInstanceOf[Rep[Array[Float]]])
      	        comp_result={
      	          predicate match {
      	            case "EQUAL" => _mm_cmpeq_ps(read_val, comp_val)
      	            case "GREATER" => _mm_cmpgt_ps(read_val, comp_val)
    	            case "GREATER_EQUAL" => _mm_cmpge_ps(read_val, comp_val) 
    	            case "LESSER" =>  _mm_cmplt_ps(read_val, comp_val)
    	            case "LESSER_EQUAL" => _mm_cmple_ps(read_val, comp_val)
    	            case "NOT_EQUAL" =>  _mm_cmpneq_ps(read_val, comp_val) 
    	            case _ => _mm_cmpeq_ps(read_val,read_val) //Always true, not by operator but by parameters
    		      }
      	        }
    		    var mask: Rep[Int]= _mm_movemask_ps(comp_result)
    		    if(bfc){
    		      input(varIntToRepInt(outputPos)+displacementVal)=varIntToRepInt(basetidToWrite).asInstanceOf[Rep[Float]]
    		      basetidToWrite+=1
    		      outputPos+=(bitwiseCompare(mask,1)) 
    		      input(varIntToRepInt(outputPos)+displacementVal)=varIntToRepInt(basetidToWrite).asInstanceOf[Rep[Float]]
    		      basetidToWrite+=1
    		      outputPos+=(bitwiseCompare(binaryRightShift(mask,1), 1))
    		      input(varIntToRepInt(outputPos)+displacementVal)=varIntToRepInt(basetidToWrite).asInstanceOf[Rep[Float]]
    		      basetidToWrite+=1
    		      outputPos+=(bitwiseCompare(binaryRightShift(mask,2), 1)) 
    		      input(varIntToRepInt(outputPos)+displacementVal)=varIntToRepInt(basetidToWrite).asInstanceOf[Rep[Float]]
    		      basetidToWrite+=1
    		      outputPos+=(bitwiseCompare(binaryRightShift(mask,3), 1)) 
    		    }
    		    else{
    		      if((bitwiseCompare(mask,1)).asInstanceOf[Rep[Boolean]]){
    		        input(varIntToRepInt(outputPos)+displacementVal)=varIntToRepInt(basetidToWrite).asInstanceOf[Rep[Float]]
    		        outputPos+=1
    		      }
    		      basetidToWrite+=1
    		      if((bitwiseCompare(binaryRightShift(mask,1), 1)).asInstanceOf[Rep[Boolean]]){
    		        input(varIntToRepInt(outputPos)+displacementVal)=varIntToRepInt(basetidToWrite).asInstanceOf[Rep[Float]]
    		        outputPos+=1    		        
    		      }
    		      basetidToWrite+=1
    		      if((bitwiseCompare(binaryRightShift(mask,2), 1)).asInstanceOf[Rep[Boolean]]){
    		        input(varIntToRepInt(outputPos)+displacementVal)=varIntToRepInt(basetidToWrite).asInstanceOf[Rep[Float]]
    		        outputPos+=1
    		      }
    		      basetidToWrite+=1
    		      if((bitwiseCompare(binaryRightShift(mask,3), 1)).asInstanceOf[Rep[Boolean]]){
      		        input(varIntToRepInt(outputPos)+displacementVal)=varIntToRepInt(basetidToWrite).asInstanceOf[Rep[Float]]
    		        outputPos+=1
    		      }
    		      basetidToWrite+=1
    		    }
            }
    	    
    	    for(i <-((sse_array_length*sizeofm128/sizeoffloat)+basetid until maxNumIt): Rep[Range]){ 
              runInstructionOfIteration(i, displacementForIterationPos)
            }

    	    outputPos
    	    
    	    //Notes for combining vectorization with other variants: ALIGNMENT
    		/* My guess is that the alignment will be processed here, always. For this I can define a butFirst function, 
    		 * which would take the first part of the iteration, if needed. Other loop will simply decorate this, except here.
    		 * If done, start position must be checked for all the others...
    		 * The comparison inside can be supported for both cases, with or without branches.*/
    	    
    	    //SIMD EXECUTION
    	    /*The code for this generation is already in preparation.*/
    		/*If there is previous or succesive unrolling, it will only concern this loop, and will be limited to 2 or 4 in our support.
    		 * Most likely just 4 would be ok. No residues will have to be considered.
    		 *This will be activated by flag.
    	     * 
    	     * Parallelization would only refer to this loop as well. Residuals could be an issue.
    	     * 
    	     * The comparison inside can be supported for both cases, with or without branches.*/
    	    
    	    //RESIDUAL ITERATIONS
    	    /*Another function will have to be used for this, again this will be a function called atLast, handling only the residual
    	     *iterations of this. This would require changing all other functions, to handle the new upper bound.
    	     *The comparison inside can be supported for both cases, with or without branches.*/
    		        	        	    
    	    }  	
    	  }
    		  
    	}
    		
       	/*Class implementing GPU parallelization*/
    	class CudaLoop (decoratedLoop: SimpleLoop) extends LoopDecorator (decoratedLoop) {
    	  
    	  /*Note: Overriden functions at this point are still empty, and are just included as a working template*/
    	  override def runInstructionOfIteration(it: Rep[Int], dispIt:Rep[Int])= comment("decorated instruction", verbose = true){
    	    decoratedLoop.runInstructionOfIteration(it, dispIt)
    	  }
    			
    	  override def runIteration(it: Rep[Int], instOfIt:(Rep[Int],Rep[Int])=>Unit)= comment("decorated iteration", verbose = true){
    		decoratedLoop.runIteration(it, runInstructionOfIteration)
    	  }
    	  
    	  override def innerRunLoop(numIn:Int, numItU:Rep[Int], iteration:(Rep[Int],(Rep[Int],Rep[Int])=>Unit)=>Unit, instOfIt:(Rep[Int],Rep[Int])=>Unit, isbfc: Boolean): Var[Int] = {
    	    decoratedLoop.innerRunLoop(numIn, numItU, runIteration, runInstructionOfIteration, decoratedLoop.bfc)
    	  }
    			
    	  override def runLoop(): Var[Int]={
    	    innerRunLoop(decoratedLoop.numInst, decoratedLoop.numIterationsUnrolled, runIteration, runInstructionOfIteration, bfc)
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
    	var outputPos: Rep[Int]=varIntToRepInt(loop.runLoop()) 
		    
    	/*Printing of the output array*/
    	
    	println("Number of tuples found: ")	
    	println(outputPos)
    	
    	if(outputPos!=zero){
    		println("Output array: ")
    		var baseValue:Rep[Int]= loop.displacementForIterationPos+loop.numIterations
    		for (i <- (0 until outputPos): Rep[Range]) {
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
    
    if (instructions.contains("Vectorize")){
      if(fileLines.filter(x=>x.contains("&(")).length>0){ 
        var displacementLine= fileLines.filter(x=>x.contains("&(")).head
        var displacementLineSplit = displacementLine.split("&")
        var firstVariableName=displacementLineSplit(1).replace("(", "").replace(")", "").replace(";","")
        var substitutionLine=fileLines.filter(x=>x.contains(firstVariableName+" = ")).head
        var substitutionLineSplitted=substitutionLine.split("=")
        var substitutionString=substitutionLineSplitted(1).replace(";", "").replace(" ", "")
        var sseArrayLine = fileLines.filter(x=>x.contains("&(")).tail.head
        var sseArrayLineSplitted = sseArrayLine.split("&")
        var secondVariableName=sseArrayLineSplitted(1).replace("(", "").replace(")", "").replace(";","")
        var substitutionLine2=fileLines.filter(x=>x.contains(secondVariableName+" = ")).head
        var substitutionLine2Splitted= substitutionLine2.split("=")
        var substitutionString2=substitutionLine2Splitted(1).replace(";", "").replace(" ", "")
        fileLines=fileLines diff List(substitutionLine)
        fileLines=fileLines diff List(substitutionLine2)
        fileLines=fileLines.updated(fileLines.indexOf(displacementLine), displacementLine.replace(firstVariableName, substitutionString).replace("float*", "__m128*").replace("=", "= (__m128*)"))
        fileLines=fileLines.updated(fileLines.indexOf(sseArrayLine), sseArrayLine.replace(secondVariableName, substitutionString2).replace("=", "= (float*)"))
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
    if(instructions.contains("Parallelize") && fileLines.filter(x=>x.contains("x0[2]")).length>0){ 
    	val numThreadsLine=fileLines.filter(x=>x.contains("x0[2]")).head
    	var arrayOfSplitting: Array[java.lang.String] = numThreadsLine.split(" ")
   	   	val numThreadsIdentifier=arrayOfSplitting(3)
   	   	val loopHeaders= fileLines.filter(x=> x.contains("for(")).filter(x=> x.contains("< "+numThreadsIdentifier))
   	   	if (loopHeaders.length>0){
   	   		val prefixSumHeader=loopHeaders(0)
   	   		val parallelWritingHeader=loopHeaders(2)
   	   		arrayOfSplitting= prefixSumHeader.split("=0")
   	   		arrayOfSplitting=arrayOfSplitting(0).replace("  for(","").split(" ")
   	   		val integerRepresentation= arrayOfSplitting(0)
   	   		val iteratorName_prefixSum=arrayOfSplitting(1)
   	   		arrayOfSplitting = parallelWritingHeader.split("=0")
   	   		arrayOfSplitting=arrayOfSplitting(0).replace("  for(","").split(" ")
   	   		val iteratorName_parallelWriting=arrayOfSplitting(1)	
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
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

OvGU, January-March 2015.
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

	BRANCH REMOVAL: Only one call of remove branching is effectful. More calls dont add anything new to the execution.
	A scheme for branch removal only on selected threads could be supported, but we're not sure on the uses/benefits of this.

	LOOP UNROLLING: Loop unrolling can be added up, until only 1 iteration is left. After that, no further unrolling will have an effect.
	A scheme for different loop unrolling only on selected threads could be supported, but we're not sure on the uses/benefits of this.

	PARALLELIZATION: As it is implemented, only the last call for applying parallelization is effectful. 
	Adding more threads than there are iterations will have no further effect than a per-iteration parallelization.
	Only one form of parallelization is supported for now (parallelism for each resulting iteration or set of iterations).
	Later more levels of parallelization could be explored, if there is an interest for this. 
	
	VECTORIZATION:?

*********************************************************************************************************************************************
Working Notes:

Some ideas for parallelization can be found below, as comments to the code. 

Ideas for vectorization: ?

If not possible to generate the code for parallelization or vectorization with LMS, perhaps it could be added by getting LMS to output some
special strings to mark where parallelization should be inserted, and then post-processing the generated code with text insertion and deletion. 
If this is accomplished, then it could be further wrapped in an outer Scala class, so as to have a single call to the code-generator, and it's use 
of processing and post-processeing wouldn't be noticed by the final system.


*********************************************************************************************************************************************
Future work:
1) Embed test in cleaner function, taking input from console, so as to be usable by an existing system.
2) Design and apply a clear scheme for naming the generated codes according to variants used, or to other requirements from existing system.
3) Create copies of this that generate code for vectors of ints and other types of data.
------------------------------

*/

package scala.lms.scan_variants

import scala.virtualization.lms.common._

class ScanVariantsTests extends TutorialFunSuite {

  val under = "c_code_generation_tests_"

  test("ScanVariants") {
  
   /**Future work: This should be embedded in a cleaner function, taking input from console, so as to be usable by an existing system. **/
  
   /*Configuration for the variants to be applied*/
   var numInstructionsForUnrolling: Int=4;
   var instructions = new Array[String](numInstructionsForUnrolling); 
   instructions(0) = "Remove branching";  
   instructions(1) = "Unroll"; 
   instructions(2) = "Unroll"; //"Parallelize";
   instructions(3) = "Vectorize";
   var unrollDepth: Int=4
   var vectorSize: Int=4
   var numThreadsSelected: Int=1

   /*Configuration for the predicate*/
   val equals = false;
   val greaterThan=true;
   val greaterThanEquals=false;
   val lesserThan=false;
   val lesserThanEquals=false;
   val notEqual=false;

   var predicate: String ="scan_all";

   if (equals){
	predicate="equals";
   }
   else if (greaterThan){
	predicate="greaterThan";
   }
   else if (greaterThanEquals){
	predicate="greaterThanEquals";
   }
   else if (lesserThan){
	predicate="lesserThan";
   }
   else if (lesserThanEquals){
	predicate="lesserThanEquals";
   }
   else if (notEqual) {
        predicate="notEquals";
   }
   else {
        predicate="scan_all";
   }


   /*Definition of the snippet for code-generation, using a slightly modified version of EPFL's LMS DSL api for C-code generation*/		
   val snippet = new DslDriverC[Array[Float],Unit] {

	/*Area where context is shared between 2 stages: the code-generation stage and the execution stage*/


	/*Snippet function defining the function whose code is to be generated
	Takes as input an array of floats. By convention, the first parameter is the value for comparison, the second parameter is the size of the input array, 
	the following parameters are the input array and then there is a set of empty records of the same size as the input array, for storing the results...**/

	def snippet(input: Rep[Array[Float]]) = comment("Scan Variants- timer goes here", verbose = true) {

		/*Input values*/
		lazy val maxNumIt:Rep[Int]=input(1).asInstanceOf[this.Rep[Int]];
   		lazy val valueForComparison:Rep[Float]=input(0); 

		/*Local context variables*/
		var outputPos: this.Variable[Int]=2 //Definition of the output position. It is by default, displaced by 2.
		var one: this.Variable[Int]=1 //Definition of number 1, for typing purposes.


		/*For ease of work, and after observing some unexpected typing issues (and forward references problems), we decided to place here the definition of 
		the loop classes, in charge of handling the application of the variants.*/

		/**Definition of the Abstract Loop Representation class, in charge of handling the application of the variants. */
		abstract class AbstractLoopRepresentation {
			/*Inner members*/
			var numIterations: Exp[Any] =_; //Number of interations of original loop
			var numIterationsUnrolled: Rep[Int] =_; //Number of Iterations of unrolled loop
			var numThreads: Rep[Int] =_; //Number of threads
			var value: Exp[Any] =_; //Value for comparison
			var predicateType: String=_;//Predicate type
			var numInst: Int =_; //Number of instructions per iteration. 1 by default.
			var bfc: Boolean=_;//Flag to define if using branch-free-code. False by default.
			var unrolled: Boolean=_;//Flag to define if loop has been unrolled. False by default.
			var parallelized: Boolean=_;//Flag to define if loop has been parallelized. False by default.

			/*Parametric constructor. Takes as input the number of iterations, compare value and predictate.*/
			def this (numIt: Rep[Int], compareValue: Rep[Float], pred:String){
				this();
				numIterations=numIt;
				numIterationsUnrolled=numIt;
				value=compareValue;
				predicateType=pred;
				numThreads=varIntToRepInt(one);
				
				numInst=1;
				bfc=false;
				unrolled=false;
				parallelized=false;
			}

			/*Some getters...*/
			def getPredicateType():String={
				predicateType
			}
			def getNumIterations():Rep[Int]={
				numIterationsUnrolled
			}
			def getNumThreads():Rep[Int]={
				numThreads
			}
			def getNumInstructionsPerUnrolledIteration(): Int={
				numInst
			}
			def isUnrolled():Boolean={
				unrolled
			}

			/*Function in charge of applying variant changes to current loop configuration.*/
			def applyVariant(instruction: String){
				if (instruction=="Remove branching"){
					bfc=true;
				}
				else if (instruction=="Unroll"){
					unrolled=true;
					numIterationsUnrolled=(numIterationsUnrolled/unrollDepth).asInstanceOf[Rep[Int]]
					if (numIterationsUnrolled==0){
						numIterationsUnrolled=varIntToRepInt(one);
					}
					else{
						numInst=(numInst*(unrollDepth.asInstanceOf[Int])); /*The number of instructions only changes if there is more than one 
													iteration on the remaining loop.*/
					}
				}
				else if (instruction=="Parallelize"){
					parallelized=true;
					numThreads=numThreadsSelected.asInstanceOf[Rep[Int]]
				}
			}

			/*runLoop: Function responsible for running the loop. 
			Parallelization changes can be handled from here.*/
			def runLoop(){

				if (!parallelized){
					/*Loop for code generation*/
			        	for (i <- (0 until numIterationsUnrolled): Rep[Range]) {
			        	  this.runInstructionOfIteration(i)
			        	}
				}
				else{
					/*For parallelization: 
						0. Assign a load-balanced number of iterations (numInterationsUnrolled/numThreads) to each thread.
						Assign to the last the remaining iterations, except the residue from the unrolling.

						1. Run and get results of the first stage of prefix-sum. 
						A copy of the runInstructionOfIteration function should be done for this task.
						
						2. Assign positions and call the loop again. 
						Another copy of the runInstructionOfIteration function might be required for this task.
						
						Note: All these copies are not the most elegant solution, but for the time being they keep the
						code-generator simpler by not overlapping the variants.
					
					*/
				}

				/*Code generation for a non-optimized loop with the residual iterations after unrolling*/
				if (unrolled && (((numIterationsUnrolled-1)*numInst)+2+numInst)<(maxNumIt+2)){
					for (i <- ( (((numIterationsUnrolled-1)*numInst)+2+numInst) until maxNumIt+2): Rep[Range]) {
		        		  	this.runInstructionUnrollResidue(i)//Note the invocation to the residual instruction
        				}		
				}

			}

			/*Function that defines the series of instructions to be executed in one iteration of the resulting
			loop.
			Takes as input the iteration number.
			It handles mapping from iteration number to input & output arrays, considering variants performed.*/
			def runInstructionOfIteration(it: Rep[Int])= comment("run instruction", verbose = true){

				var currInst:Int=0;
				while (currInst<numInst){
					var itVal:Rep[Int]=((it*numInst)+currInst)+2
					if (bfc){
						if (predicate=="equals"){
								//println(input(itVal))
								input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
								outputPos+=(input(itVal)==value.asInstanceOf[Rep[Float]]).asInstanceOf[Rep[Int]];
						}
   						else if (predicate=="greaterThan"){
								input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
				                		outputPos+=(input(itVal)>value.asInstanceOf[Rep[Float]]).asInstanceOf[Rep[Int]];
   						}	
						else if (predicate=="greaterThanEquals"){
								input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
				                		outputPos+=(input(itVal)>=value.asInstanceOf[Rep[Float]]).asInstanceOf[Rep[Int]];
   						}
						else if (predicate=="lesserThan"){
								input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
				                		outputPos+=(input(itVal)<value.asInstanceOf[Rep[Float]]).asInstanceOf[Rep[Int]];   		   							}
						else if (predicate=="lesserThanEquals"){
								input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
				                		outputPos+=(input(itVal)<=value.asInstanceOf[Rep[Float]]).asInstanceOf[Rep[Int]];	
	   					}
						else if (predicate=="notEquals"){
								input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
				                		outputPos+=(input(itVal)!=value.asInstanceOf[Rep[Float]]).asInstanceOf[Rep[Int]];
	      					}
		   				else {
								input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
								outputPos+=1;
						}
					}
					else if (predicate=="equals"){
						if (input(itVal)==value.asInstanceOf[Rep[Float]]){
							input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
							outputPos+=1;
						}
					}
   					else if (predicate=="greaterThan"){
						if (input(itVal)>value.asInstanceOf[Rep[Float]]){
							input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
							outputPos+=1;
						}
   					}	
					else if (predicate=="greaterThanEquals"){
						if (input(itVal)>=value.asInstanceOf[Rep[Float]]){
							input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
							outputPos+=1;
						}
   					}
					else if (predicate=="lesserThan"){
						if (input(itVal)<value.asInstanceOf[Rep[Float]]){
							input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
							outputPos+=1;					
						}
   					}	
					else if (predicate=="lesserThanEquals"){
						if (input(itVal)<=value.asInstanceOf[Rep[Float]]){
							input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
							outputPos+=1;					
						}
   					}
					else if (predicate=="notEquals"){
						if (input(itVal)!=value.asInstanceOf[Rep[Float]]){
							input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
							outputPos+=1;
						}
   					}
   					else {
						input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
						outputPos+=1;
					}
				
					currInst=currInst+1;
				} //End of while loop		
			}//End of def runInstructionOfIteration

			/*Function that defines the instructions to be executed for the residue left after applying unrolling, without
			uneven instructions per iteration. 
			Takes as input the absolute positions of tuples not visited.
			It handles mapping from iteration number to input & output arrays, considering variants performed.*/
			def runInstructionUnrollResidue(itVal: Rep[Int])= comment("run residue instructions after unroll", verbose = true){
				if (bfc){
					if (predicate=="equals"){
							input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
							outputPos+=(input(itVal)==value.asInstanceOf[Rep[Float]]).asInstanceOf[Rep[Int]];
					}
   					else if (predicate=="greaterThan"){
							input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
			                		outputPos+=(input(itVal)>value.asInstanceOf[Rep[Float]]).asInstanceOf[Rep[Int]];
   					}	
					else if (predicate=="greaterThanEquals"){
							input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
			                		outputPos+=(input(itVal)>=value.asInstanceOf[Rep[Float]]).asInstanceOf[Rep[Int]];
  					}
					else if (predicate=="lesserThan"){
							input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
			                		outputPos+=(input(itVal)<value.asInstanceOf[Rep[Float]]).asInstanceOf[Rep[Int]];   		   						}
					else if (predicate=="lesserThanEquals"){
							input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
			                		outputPos+=(input(itVal)<=value.asInstanceOf[Rep[Float]]).asInstanceOf[Rep[Int]];	
   					}
					else if (predicate=="notEquals"){
							input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
			                		outputPos+=(input(itVal)!=value.asInstanceOf[Rep[Float]]).asInstanceOf[Rep[Int]];
      					}
	   				else {
							input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
							outputPos+=1;
					}
				}
				else if (predicate=="equals"){
					if (input(itVal)==value.asInstanceOf[Rep[Float]]){
						input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
						outputPos+=1;
					}
				}
   				else if (predicate=="greaterThan"){
					if (input(itVal)>value.asInstanceOf[Rep[Float]]){
						input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
						outputPos+=1;
					}
   				}	
				else if (predicate=="greaterThanEquals"){
					if (input(itVal)>=value.asInstanceOf[Rep[Float]]){
						input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
						outputPos+=1;
					}
   				}
				else if (predicate=="lesserThan"){
					if (input(itVal)<value.asInstanceOf[Rep[Float]]){
						input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
						outputPos+=1;					
					}
   				}		
				else if (predicate=="lesserThanEquals"){
					if (input(itVal)<=value.asInstanceOf[Rep[Float]]){
						input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
						outputPos+=1;					
					}
  				}
				else if (predicate=="notEquals"){
					if (input(itVal)!=value.asInstanceOf[Rep[Float]]){
						input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
						outputPos+=1;
					}
				}
   				else {
					input(varIntToRepInt(outputPos)+maxNumIt)=input(itVal)
					outputPos+=1;
				}
			}//End of def runInstructionUnrollResidue

		} //End of Abstract Loop Representation	

		/*Dummy class that enforces the use of parametric constuctor.*/
		class LoopRepresentation (maxNumIt: Rep[Int], valueForComparison: Rep[Float], predicate:String) extends AbstractLoopRepresentation(maxNumIt,valueForComparison,predicate){
		}


 		/*Initialization of the iteration space*/       
		val iterationSpace = new LoopRepresentation(maxNumIt, valueForComparison, predicate)

		/*Application of variants pased as an array of strings, and values of configuration variables*/
		for (instruction<-instructions){
			iterationSpace.applyVariant(instruction)

			/*If we want different configurations for succesive unrolling, vectorization or parallelization, the 
			local variables determining unroll depth, number of threads and size of vectorized instructions
			should be changed here.*/
		}

		iterationSpace.runLoop();
		
		/*Printing of the output array*/	
		println("Output array: ");
		for (i <- (0 until outputPos-2): Rep[Range]) {
        	  println(input(maxNumIt+2+i));
        	}

      	}//End of snippet function

    /*End of area where context is shared between 2 stages*/

    }//End of use of DSL driver

    check("ScanVariants_"+predicate, snippet.code, "c")     /*The naming scheme can be modified in this line*/
    
  } //End of test "Scan Variants"
} //End of class ScanVariantsTest


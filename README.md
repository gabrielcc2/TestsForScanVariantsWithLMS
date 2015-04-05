# TestsForScanVariantsWithLMS

In these tests we use Light-weight modular staging (LMS) (http://scala-lms.github.io/) and their version of a DSL api (https://github.com/scala-lms/tutorials/blob/master/src/test/scala/lms/tutorial/dslapi.scala, with some minor modifications of our own), to generate C code for different optimizations of the Scan operation. We are specifically studying how code generation could be carried out for loop unrolling, branch free code, vectorization (using the X86 instructions), parallelization (using phthreads) and their combinations. GPU parallelization has been added, but without support for combining it with other variants.

GPU parallelization is still not included in this release.

A decorator pattern is proposed for implementing the variants, allowing to combine them commutatively.

Our proposal for the parallelized variant relies on post-processing generated code to include pthread instructions and re-order code to comply with thread use. 

Vectorization has relied on our definition of some staged operators (included here: https://github.com/gabrielcc2/TestsForScanVariantsWithLMS/blob/master/virtualization-lms-core/src/common/MiscOps.scala). Post-processing of the generated code was also used, to provide unsupported types (__mm128), pointer handling and some castings.

To run the code you need first to install the scala build tool: sbt.

Then go to the virtualization folder and call sbt publish-local. 

Now navigate to the test-code folder and call the same instruction. 

Next, within this folder you can call sbt test. This compiles and runs the test_code/src/test/scala/lms/scan_variants/scanVariants.scala file, which generates C code for the variants according to in-file parameters. These can be changed by the user, for generating different variants and for different comparison operations.

The resulting C code can be found in the folder: test_code/src/out.

To compile use gcc with options -std=c99 -pthread.

For example, it can be compiled as follows:
gcc c_code_generation_tests_ScanVariants_GREATER_EQUAL.check.c -o test.o -std=c99 -pthread

(Note that G++ doesnt accept our parallel code, since it doesn't support defining funtions within another functions. Doing this was our decision so as to allow the new functions to access local variables. 

To change this would require a more elaborate post-processing scheme than the one we've developed so far.)

To run it you need a file consisting or a list of floats. An example input file is included.

To run the C program you pass as input, in the command line: fileName numTuples compareValue numThreads. If not parallelized, numThreads should be 0. If parallelized, numThreads should start at 1.

The compare value is the right hand value of the pre-defined predicate (equals, greater than, etc) for comparing with every tuple.

For example, it can be called as follows:
./test.o inputFile.txt 20 26 7

Some examples of the generated code can be found in the folder "Examples of generated code"

If there is any problem installing sbt or configuring the code for LMS, please refer to: https://github.com/epfldata/lms

Upcoming work:
- Complete and include report on this.

- Test for execution time.

Optional work for deployment:
- Embed generator in cleaner function, taking input from console, so as to be usable by an existing system.
- Design and apply a clear scheme for naming the generated codes according to variants used, or to other requirements from existing system.
- ...

Ideas for Future Work:
- Extend the vectorization so it supports other vector sizes (our release only supports a vector of size 4).
- Consider alternatives for generating the parallel code, such as using the Delite framework (http://stanford-ppl.github.io/Delite/).
- Test for other operators.

Possible improvements of our proposal:
- Remove possible functional redundancies introduced when overriding functions with the decorator pattern.
- In order to support vectorization, some functions for mapping were implemented in the scala.virtualization.lms.common.MiscOps file. However they were implemented without reflection (since using that would imply using Rep[] as input, which then would be mapped to C with quotation marks or with the Const() transformator). It would be of interest to evaluate if the reflective effects would be needed, if so, the Rep[] input should be used, and the small errors of using quotation marks of the Const() transformator have to be addressed by post-processing the generated code, or another method.
- Generate parallelism not through post-processing but through defining staged operators and emitting a function (https://github.com/TiarkRompf/virtualization-lms-core/blob/develop/src/common/Functions.scala), albeit inlined at call site. Perhaps a workaround for this inlining issue could be implemented, so as to make the generated accepted by more compilers.
- Reduce use of post-processing by extending types and functions of the virtualization library.
- Use template type for the input variables.

Note on testing:
- Some simple, yet combinatorial-exhaustive testing with the 4 variants in different orders was carried out as a proof of validity of this code-generating library (please note that the examples of the code generated for this are included in the repository). However, the possible effects of variations in the unrollDepth or number of tuples, were not tested. If there are possible bugs in our proposal, they might relate to those un-tested cases.

# TestsForScanVariantsWithLMS

In these tests we use Light-weight modular staging (LMS) (http://scala-lms.github.io/) and their version of a DSL api (https://github.com/scala-lms/tutorials/blob/master/src/test/scala/lms/tutorial/dslapi.scala, with some minor modifications of our own), to generate C code for different optimizations of the Scan operation. We are specifically studying how code generation could be carried out for loop unrolling, branch free code, vectorization (using the X86 instructions), parallelization (using phthreads) and their combinations. GPU parallelization has been added, but without support for combining it with other variants.

Vectorization and GPU parallelization are still not included in this release.

Support for thread parallelism has worked so far by post-processing generated code, and not by defining new staged-operators in the LMS library.

To run the code you need first to install the scala build tool: sbt.

Then go to the virtualization folder and call sbt publish-local. 

Now navigate to the test-code folder and call the same instruction. 

Next, within this folder you can call sbt test. This compiles and runs the test_code/src/test/scala/lms/scan_variants/scanVariants.scala file, which generates C code according to in-file parameters. These can be changed by the user.

The resulting C code can be found in test_code/src/out.

To run it you need a file consisting or a list of floats.

To compile use gcc with options -std=c99 -pthread 

(Note that G++ doesnt accept our parallel code, since it doesn't support defining funtions within another functions. Doing this was our decision so as to allow the new functions to access local variables. 

To change this would require a more elaborate post-processing scheme than the one we've developed so far.)

To run the C program you pass as input, in the command line: fileName numTuples compareValue numThreads. If not parallelized, numThreads should be 0. If parallelized, numThreads should start at 1.

The compare value is the right hand value of the pre-defined predicate (equals, greater than, etc) for comparing with every tuple.

If there is any problem installing sbt or configuring the code, please refer to: https://github.com/epfldata/lms

Upcoming work:
- Complete remaining variants: Vectorization, by defining staged operators as seen in: https://github.com/TiarkRompf/virtualization-lms-core/blob/develop/src/common/BooleanOps.scala.

- Test variant generation in combinations and in different orders.

- Test for execution time for floats and ints (upload int version of the library separately).

Optional work for deployment:
- Embed generator in cleaner function, taking input from console, so as to be usable by an existing system.
- Design and apply a clear scheme for naming the generated codes according to variants used, or to other requirements from existing system.
- Generate parallelism not through post-processing but through defining staged operators and emitting a function (https://github.com/TiarkRompf/virtualization-lms-core/blob/develop/src/common/Functions.scala), albeit inlined at call site. Perhaps a workaround for this inlining issue could be implemented, so as to make the generated accepted by more compilers.
- ...

Ideas for Future Work:
- Remove possible functional redundancies introduced when overriding functions with the decorator pattern.
- Use template type for the input variables.
- Consider alternatives for generating the parallel code, such as using the Delite framework (http://stanford-ppl.github.io/Delite/).

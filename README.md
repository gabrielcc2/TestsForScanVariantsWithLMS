# TestsForScanVariantsWithLMS

In these tests we use Light-weight modular staging (LMS) (http://scala-lms.github.io/) and their version of a DSL api (https://github.com/scala-lms/tutorials/blob/master/src/test/scala/lms/tutorial/dslapi.scala, with some minor modifications of our own), to generate C code for different optimizations of the Scan operation. We are specifically studying how code generation could be carried out for loop unrolling, branch free code, vectorization (using the X86 instructions), parallelization (using phthreads) and their combinations.

Vectorization is still not included in this release.

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
- Correct current found bugs:
  - Thread library bug: Occasional different values observed in repeated runs with same input+configuration.
  - Bogus behaviour with parallelization when the division: number of iterations/number of threads, is inexact.
- Standardize processing of residual iterations for both unrolled loops and the inexact division described above.

- Test variant generation in combinations: 
  - Parallelized+ Check inexact cases, behaviour at limits: Previously passed with bugs.
  - Unrolled+ Check inexact cases, behaviour at limits: Previously Ok.
  - BFC: Previously Ok.
  - Parallelized and Unrolled+ Check inexact cases for both, behaviour at limits: Previously passed with bugs.
  - Parallelized and BFC+ Check inexact cases, behaviour at limits: Previously passed with bugs.
  - Unrolled and BFC+ Check inexact cases, behaviour at limits: Previously Ok.
  - Parallelized and Unrolled and BFC+ Check inexact cases for Unrolled and Parallelized, behaviour at limits: Previously passed with bugs.

- Correct, if any, issues with behaviour at limits.

- Check if it's perhaps better to generate a parallelization at specific thread number instead of passing this as parameter. Change accordingly.

- Complete remaining variants: Vectorization, by defining staged operators as seen in: https://github.com/TiarkRompf/virtualization-lms-core/blob/develop/src/common/BooleanOps.scala.

- Re-test
- Clean generators code.
- Study how to further clean the generated code.
- Test for execution time.
- Study potential improvements + additional configurability options, if fitting. 

Optional work for deployment:
- Embed generator in cleaner function, taking input from console, so as to be usable by an existing system.
- Design and apply a clear scheme for naming the generated codes according to variants used, or to other requirements from existing system.
- Extend this to generate code for vectors of ints and other types of data (so far only float is supported).
- Generate parallelism not through post-processing but through defining staged operators and emitting a function (https://github.com/TiarkRompf/virtualization-lms-core/blob/develop/src/common/Functions.scala), albeit inlined at call site. Perhaps a workaround for this inlining issue could be implemented, so as to make the generated accepted by more compilers.
- ...

Ideas for Future Work:
- Consider alternatives for generating the parallel code, such as using the Delite framework (http://stanford-ppl.github.io/Delite/).

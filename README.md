# TestsForScanVariantsWithLMS

In these tests we use Light-weight modular staging (LMS) (http://scala-lms.github.io/) and their version of a DSL api (provided in the LMS tutorials site, with some minor modifications of our own), to generate C code for different optimizations of the Scan operation. Basically we are trying to study how code generation could be carried out for loop unrolling, branch free code, vectorization and parallelization using phthreads.

Vectorization is still not included in this release.

To run the code you need first to install the scala build tool: sbt.

Then go to the virtualization folder and call sbt publish-local. 

Now navigate to the test-code folder and call the same instruction. 

Next, within this folder you can call sbt test. This compiles and runs the test_code/src/test/scala/lms/scan_variants/scanVariants.scala file, which generates C code according to in-file parameters. These can be changed according to user demands.

The resulting C code can be found in test_code/src/out.

To run it you need a file consisting or a list of floats.

To compile use gcc with options -std=c99 -pthread 

(Note that G++ doesnt accept our parallel code, since we do a definition of functions from within another functions. This was our decision so as to allow the new functions to access local variables. 

To change this would require a more elaborate post-processing scheme than the one we've developed.)

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
- Complete remaining variants: Vectorization
- Re-test
- Clean generators code.
- Study how to further clean the generated code.
- Test for execution time.
- Study potential improvements + additional configurability options, if fitting. 

Optional work for deployment:
- Embed generator in cleaner function, taking input from console, so as to be usable by an existing system.
- Design and apply a clear scheme for naming the generated codes according to variants used, or to other requirements from existing system.
- Create copies of this that generate code for vectors of ints and other types of data (so far only float is supported).
- ...

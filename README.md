# TestsForScanVariantsWithLMS

In these tests we use Light-weight modular staging (LMS) (http://scala-lms.github.io/) and their version of a DSL api (provided in the LMS tutorials site, with some minor modifications of our own), to generate C code for different optimizations of the Scan operation. Basically we are trying to study how code generation could be carried out for loop unrolling, branch free code, vectorization and parallelization using phthreads.

The last 2 are still not included in this release.

To run the code you need first to install the scala build tool: sbt.

Then go to the virtualization folder and call sbt publish-local. 

Now navigate to the test-code folder and call the same instructions. 

Next, within this folder you can call sbt test. This compiles and runs the test_code/src/test/scala/lms/scan_variants/scanVariants.scala file, which generates C code according to in-file parameters. These can be changed according to user demands.

The resulting C code can be found in test_code/src/out.

To run it you need a file consisting or a list of floats.

To compile use gcc with options -std=c99 -pthread

To run the C program you pass as input, in the command line: fileName numTuples compareValue numThreads

The compare value is the right hand value of the pre-defined predicate (equals, greater than, etc) for comparing with every tuple.

If there is any problem installing sbt or configuring the code, please refer to: https://github.com/epfldata/lms

Upcoming work:
- Correct current found bugs:
  - Thread library bug: Occasional different values observed in repeated runs with same input+configuration.
  - Bogus behaviour with parallelization and inexact division number of iterations/number of threads.
- Standardize processing of residual iterations.

- Test in combinations: 
  - Parallelized+ Check inexact cases: Previously passed with bugs.
  - Unrolled+ Check inexact cases: Previously Ok.
  - BFC: Previously Ok.
  - Parallelized and Unrolled+ Check inexact cases for both: Previously passed with bugs.
  - Parallelized and BFC+ Check inexact cases: Previously passed with bugs.
  - Unrolled and BFC+ Check inexact cases: Previously Ok.
  - Parallelized and Unrolled and BFC+ Check inexact cases for Unrolled and Parallelized: Previously passed with bugs.

- Complete remaining variants: Vectorization
- Re-test
- Add error checking to C code and generator.
- Clean generators code.
- Study how to further clean the generated code.
- Test for execution time.
- Propose improvements + additional configurability options, if fitting. 
- ...

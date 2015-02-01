# TestsForScanVariantsWithLMS

In these tests we use Light-weight modular staging (LMS) (scala-lms.github.io) and their version of a DSL api (provided in the LMS tutorials site, with some minor modifications of our own), to generate C code for different optimizations of the Scan operation. Basically we are trying to study how code generation could be carried out for loop unrolling, branch free code, vectorization and parallelization using phthreads.

The last 2 are still not included in this release.

To run the code you need first to install the scala build tool: sbt.

Then go to the virtualization folder and call sbt publish-local. 

Now navigate to the test-code folder and call the same instructions. 

Next, within this folder you can call sbt test. This compiles and runs the test_code/src/test/scala/lms/scan_variants/scanVariants.scala file, which generates C code according to in-file parameters. These can be changed according to user demans.

The resulting C code can be found in test_code/src/out.


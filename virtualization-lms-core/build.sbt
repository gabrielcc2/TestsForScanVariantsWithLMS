name := "LMS"

version := "0.3-SNAPSHOT"

organization := "EPFL"

scalaOrganization := "org.scala-lang.virtualized"

scalaVersion := virtScala

scalaSource in Compile <<= baseDirectory(_ / "src")

scalaSource in Test <<= baseDirectory(_ / "test-src")

scalacOptions += "-Yvirtualize"

//scalacOptions += "-Yvirtpatmat"

//scalacOptions in Compile ++= Seq(/*Unchecked, */Deprecation)


libraryDependencies += "org.scala-lang.virtualized" % "scala-library" % virtScala

libraryDependencies += "org.scala-lang.virtualized" % "scala-compiler" % virtScala

libraryDependencies += scalaTest


// tests are not thread safe
parallelExecution in Test := false

// disable publishing of main docs
publishArtifact in (Compile, packageDoc) := false


// continuations
autoCompilerPlugins := true

addCompilerPlugin("org.scala-lang.virtualized.plugins" % "continuations" % virtScala)

scalacOptions += "-P:continuations:enable"

/* CI Specific Tests, that output JUnit test output. */

let jUnitOutputFile = "./" ++ Sys.os_type ++ "_junit.xml";

Generic_Fuzzy_Test.TestFramework.run(
  Rely.RunConfig.withReporters(
    [Default, JUnit(jUnitOutputFile)],
    Rely.RunConfig.initialize(),
  ),
);

Path_Fuzzy_Test.TestFramework.run(
  Rely.RunConfig.withReporters(
    [Default, JUnit(jUnitOutputFile)],
    Rely.RunConfig.initialize(),
  ),
);
ReasonFuzzBench.BenchFramework.cli();
Console.log("");

Generic_Fuzzy_Test.TestFramework.cli();
let result = ReasonFuzz.pathFuzzyMatch(~line="ax", ~pattern="a");
Console.log(result);
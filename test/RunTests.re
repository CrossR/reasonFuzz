Generic_Fuzzy_Test.TestFramework.cli();
let result = ReasonFuzz.pathFuzzyMatch(~line="abc", ~pattern="abx");
Console.log(result);
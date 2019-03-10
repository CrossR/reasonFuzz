Generic_Fuzzy_Test.TestFramework.cli();
let result = ReasonFuzz.pathFuzzyMatch(~line="axbycz", ~pattern="abc");
Console.log(result);
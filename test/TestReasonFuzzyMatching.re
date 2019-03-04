let result = ReasonFuzzyMatching.Fuzzy.fuzzyMatch("abc", "abx");
Console.log(result);

let result2 = ReasonFuzzyMatching.Fuzzy.fuzzyMatch("axbycz", "abc");
Console.log(result2);

let result3 = ReasonFuzzyMatching.Fuzzy.fuzzyMatch("axbycz", "xyz");
Console.log(result3);

let result4 = ReasonFuzzyMatching.Fuzzy.fuzzyIndicies("axbycz", "abc");
Console.log(result4)
Console.log("Should be [0, 2, 4].")

Console.log(
    ReasonFuzzyMatching.Fuzzy.fuzzyIndicies("browser/src/index.ts", "src")
);
Console.log("Should be [8, 9, 10].")

Console.log(
    ReasonFuzzyMatching.Fuzzy.fuzzyIndicies("browser/test/index.ts", "src")
);
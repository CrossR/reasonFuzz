open TestFramework;
open Generic_Fuzzy_Test;

describe("Fzy: Match scores should be correct.", ({test, _}) => {

  test("Test new lib", ({expect}) => {
    let testString = "SRC";
    let testList = [|"browser/src/index.ts", "browser/SRC/index.ts"|];
    let score = Fzy.searchForItem(testList, testString);
    expect.float(score[1]).toBeCloseTo(0.0);
    expect.float(score[0]).toBeCloseTo(2.815);
  });

  test("Test new lib on larger input", ({expect}) => {
    let testPattern = "gpio-regulator";

    let resultArray = Fzy.searchForItem(TestArray.linuxTest, testPattern);

    let bestScore = ref(0.0);
    let bestResult = ref("");

    for (x in 0 to Array.length(resultArray) - 1) {

        if (resultArray[x] > 0.0) {
          Console.log("Score " ++ string_of_int(x) ++ " was " ++ string_of_float(resultArray[x]));
        };

        if (bestScore^ < resultArray[x]) {
            bestScore := resultArray[x];
            bestResult := TestArray.linuxTest[x];
        };
    };

    /* Should fail... */
    expect.float(bestScore^).toBeCloseTo(2.0);
    expect.string(bestResult^).toEqual("");
  });
});

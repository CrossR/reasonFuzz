open TestFramework;
open Generic_Fuzzy_Test;

describe("Fzy: Match scores should be correct.", ({test, _}) => {
  test("Test fuzzy on basic test", ({expect}) => {
    let testString = "SRC";
    let testList = [|"browser/src/index.ts", "browser/SRC/index.ts"|];
    let score = Fzy.searchForItem(testList, testString);
    expect.float(score[1]).toBeCloseTo(0.0);
    expect.float(score[0]).toBeCloseTo(2.815);
  });

  test("Test fzy on larger input", ({expect}) => {
    let testPattern = "quickOpenScore";

    let resultArray = Fzy.searchForItem(TestArray.testInput, testPattern);

    let bestScore = ref(0.0);
    let bestResult = ref("");
    let bestPositions = ref([||]);

    bestScore := resultArray[0];
    bestResult := TestArray.testInput[0];
    bestPositions := [||];

    /* Expected to fail, since we only return the scores. */
    expect.float(bestScore^).toBeCloseTo(13.695);
    expect.string(bestResult^).toEqual(
      "./src/vs/base/parts/quickopen/common/quickOpenScorer.ts",
    );
    expect.array(bestPositions^).toEqual([|
      37,
      38,
      39,
      40,
      41,
      42,
      43,
      44,
      45,
      46,
      47,
      48,
      49,
      50,
    |]);
  });

  test("Test fzy on even larger input", ({expect}) => {
    let testPattern = "gpio-regulator";

    let resultArray = Fzy.searchForItem(TestArray.linuxTest, testPattern);

    let bestScore = ref(0.0);
    let bestResult = ref("");
    let bestPositions = ref([||]);

    bestScore := resultArray[0];
    bestResult := TestArray.linuxTest[0];
    bestPositions := [||];

    /* Expected to fail, since we only return the scores. */
    expect.float(bestScore^).toBeCloseTo(13.79);
    expect.string(bestResult^).toEqual(
      "./drivers/regulator/gpio-regulator.c",
    );
    expect.array(bestPositions^).toEqual([|
      20,
      21,
      22,
      23,
      24,
      25,
      26,
      27,
      28,
      29,
      30,
      31,
      32,
      33,
    |]);
  });
});

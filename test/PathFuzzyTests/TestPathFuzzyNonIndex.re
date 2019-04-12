open TestFramework;
open Generic_Fuzzy_Test;
open ReasonFuzz;

describe("Path : Match scores should be correct.", ({test, _}) => {
  test("Doesn't match index when not possible", ({expect}) => {
    let result = pathFuzzyMatch(~line="abc", ~pattern="abx");
    expect.equal(result, None);
  });

  test("Does match when possible", ({expect}) => {
    let result = pathFuzzyMatch(~line="axbycz", ~pattern="abc");
    expect.notEqual(result, None);
  });

  test("Correctly score case-match higher", ({expect}) => {
    let testString = "SRC";
    let testList = [|"browser/src/index.ts", "browser/SRC/index.ts"|];

    let result1 = pathFuzzyMatch(~line=testList[0], ~pattern=testString);
    let result2 = pathFuzzyMatch(~line=testList[1], ~pattern=testString);

    expect.notEqual(result1, None);
    expect.notEqual(result2, None);

    expect.notEqual(result1, result2);

    let score1 =
      switch (result1) {
      | Some(match) => match.score
      | None => min_int
      };

    let score2 =
      switch (result2) {
      | Some(match) => match.score
      | None => min_int
      };

    expect.equal(score2 > score1, true);
  });

  test(
    "Correctly sorts results for shortest result on file name.", ({expect}) => {
    let testPattern = "main";
    let testInputs = [|
      "packages/core/src/main.tex",
      "packages/core/test/main.tex",
      "packages/core/test/oni/main.tex",
    |];

    let resultArray = fuzzySortArray(testInputs, testPattern, pathFuzzyMatch);

    expect.equal(resultArray[0], testInputs[0]);
  });

  test("Correctly sorts results for fuzzy match.", ({expect}) => {
    let testInputs = [|
      "packages/demo/src/BufferEditor.ts",
      "packages/demo/src/BufferEditorContainer.ts",
      "packages/core/src/astBackedEditing.ts",
    |];

    let testPattern = "aBE";

    let resultArray = fuzzySortArray(testInputs, testPattern, pathFuzzyMatch);

    expect.equal(resultArray[0], testInputs[2]);
  });

  test("Works for large input", ({expect}) => {
    let testPattern = "quickOpenScore";

    let resultArray =
      fuzzySortArray(TestArray.testInput, testPattern, pathFuzzyMatch);

    expect.equal(
      resultArray[0],
      "./src/vs/base/parts/quickopen/common/quickOpenScorer.ts",
    );
  });

  test("Works for Oni src input", ({expect}) => {
    let testPattern = "token";

    let resultArray =
      fuzzySortArray(TestArray.oniTestInput, testPattern, pathFuzzyMatch);

    expect.equal(resultArray[0], "src/editor/Model/Tokenizer.re");
  });

  test("Work for even larger input", ({expect}) => {
    let testPattern = "gpio-regulator";

    let resultArray =
      fuzzySortArray(TestArray.linuxTest, testPattern, pathFuzzyMatch);

    expect.equal(resultArray[0], "./drivers/regulator/gpio-regulator.c");
  });

  test("Better match is picked", ({expect}) => {
    let result1 = pathFuzzyMatch(~line="abcxyz", ~pattern="abc");
    let result2 = pathFuzzyMatch(~line="abcxyz", ~pattern="acz");

    expect.notEqual(result1, None);
    expect.notEqual(result2, None);

    expect.notEqual(result1, result2);

    let score1 =
      switch (result1) {
      | Some(match) => match.score
      | None => min_int
      };

    let score2 =
      switch (result2) {
      | Some(match) => match.score
      | None => min_int
      };

    expect.equal(score1 > score2, true);
  });
});

open TestFramework;
open Generic_Fuzzy_Test;
open ReasonFuzz.PathMatcher;

describe("Path : Match scores should be correct.", ({test, _}) => {
  test("Doesn't match index when not possible", ({expect, _}) => {
    let result = fuzzyMatch(~line="abc", ~pattern="abx");
    expect.equal(result, None);
  });

  test("Does match when possible", ({expect, _}) => {
    let result = fuzzyMatch(~line="axbycz", ~pattern="abc");
    expect.notEqual(result, None);
  });

  test("Correctly score case-match higher", ({expect, _}) => {
    let testString = "SRC";
    let testList = [|"browser/src/index.ts", "browser/SRC/index.ts"|];

    let result1 = fuzzyMatch(~line=testList[0], ~pattern=testString);
    let result2 = fuzzyMatch(~line=testList[1], ~pattern=testString);

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
    "Correctly sorts results for shortest result on file name.", ({expect, _}) => {
    let testPattern = "main";
    let testInputs = [|
      "packages/core/src/main.tex",
      "packages/core/test/main.tex",
      "packages/core/test/oni/main.tex",
    |];

    let resultArray =
      ReasonFuzz.fuzzySortArray(testInputs, testPattern, fuzzyMatch);

    expect.equal(resultArray[0], testInputs[0]);
  });

  test("Correctly sorts results for fuzzy match.", ({expect, _}) => {
    let testInputs = [|
      "packages/demo/src/BufferEditor.ts",
      "packages/demo/src/BufferEditorContainer.ts",
      "packages/core/src/astBackedEditing.ts",
    |];

    let testPattern = "aBE";

    let resultArray =
      ReasonFuzz.fuzzySortArray(testInputs, testPattern, fuzzyMatch);

    expect.equal(resultArray[0], testInputs[2]);
  });

  test("Works for large input", ({expect, _}) => {
    let testPattern = "quickOpenScore";

    let resultArray =
      ReasonFuzz.fuzzySortArray(TestArray.testInput, testPattern, fuzzyMatch);

    expect.equal(
      resultArray[0],
      "./src/vs/base/parts/quickopen/common/quickOpenScorer.ts",
    );
  });

  test("Works for Oni src input", ({expect, _}) => {
    let testPattern = "token";

    let resultArray =
      ReasonFuzz.fuzzySortArray(
        TestArray.oniTestInput,
        testPattern,
        fuzzyMatch,
      );

    expect.equal(resultArray[0], "src/editor/Model/Tokenizer.re");
  });

  test("Work for even larger input", ({expect, _}) => {
    let testPattern = "gpio-regulator";

    let resultArray =
      ReasonFuzz.fuzzySortArray(TestArray.linuxTest, testPattern, fuzzyMatch);

    expect.equal(resultArray[0], "./drivers/regulator/gpio-regulator.c");
  });

  test("Better match is picked", ({expect, _}) => {
    let result1 = fuzzyMatch(~line="abcxyz", ~pattern="abc");
    let result2 = fuzzyMatch(~line="abcxyz", ~pattern="acz");

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

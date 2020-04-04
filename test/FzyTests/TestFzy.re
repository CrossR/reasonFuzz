open TestFramework;
open Generic_Fuzzy_Test;
open ReasonFuzz.Fzy;

describe("Fzy: Match scores should be correct.", ({test, _}) => {
  test("Works for basic test.", ({expect, _}) => {
    let testString = "SRC";
    let testList = [|"browser/src/index.ts", "browser/SRC/index.ts"|];

    let score = fzySearchArray(testList, testString);

    expect.int(Array.length(score)).toBe(1);
    expect.float(score[0].score).toBeCloseTo(2.815);
    expect.string(score[0].term).toEqual("browser/SRC/index.ts");
    expect.array(score[0].positions).toEqual([|8, 9, 10|]);
  });

  test("Works for larger test: VSCode Test", ({expect, _}) => {
    let testPattern = "quickOpenScore";

    let resultArray = fzySearchArray(TestArray.testInput, testPattern);

    let bestScore = ref(0.0);
    let bestResult = ref("");
    let bestPositions = ref([||]);

    bestScore := resultArray[0].score;
    bestResult := resultArray[0].term;
    bestPositions := resultArray[0].positions;

    expect.int(Array.length(resultArray)).toBe(2);
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

  test("Works for very large test: Linux Kernel", ({expect, _}) => {
    let testPattern = "gpio-regulator";

    let resultArray = fzySearchArray(TestArray.linuxTest, testPattern);

    let bestScore = ref(0.0);
    let bestResult = ref("");
    let bestPositions = ref([||]);

    bestScore := resultArray[0].score;
    bestResult := resultArray[0].term;
    bestPositions := resultArray[0].positions;

    expect.int(Array.length(resultArray)).toBe(3);
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

  test("Doesn't match index when not possible", ({expect, _}) => {
    let result = fzySearchArray([|"abc"|], "abx");
    expect.equal(result, [||]);
  });

  test("Does match when possible", ({expect, _}) => {
    let result = fzySearchArray([|"axbycz"|], "abc");
    expect.notEqual(result, [||]);
  });

  test(
    "Correctly sorts results for shortest result on file name.", ({expect, _}) => {
    let testPattern = "main";
    let testInputs = [|
      "packages/core/src/main.tex",
      "packages/core/test/main.tex",
      "packages/core/test/oni/main.tex",
    |];

    let result = fzySearchArray(testInputs, testPattern);

    expect.equal(result[0].term, testInputs[0]);
    expect.array(result[0].positions).toEqual([|18, 19, 20, 21|]);
  });

  test("Index match is correct", ({expect, _}) => {
    let result = fzySearchArray([|"axbycz"|], "abc");
    expect.array(result[0].positions).toEqual([|0, 2, 4|]);
  });

  test("Correctly sorts results for fuzzy match.", ({expect, _}) => {
    let testInputs = [|
      "packages/demo/src/BufferEditor.ts",
      "packages/demo/src/BufferEditorContainer.ts",
      "packages/core/src/astBackedEditing.ts",
    |];

    let testPattern = "aBE";
    let result = fzySearchArray(testInputs, testPattern);

    expect.equal(result[0].term, testInputs[2]);
    expect.array(result[0].positions).toEqual([|18, 21, 27|]);
    expect.equal(result[1].term, testInputs[0]);
    expect.array(result[1].positions).toEqual([|4, 18, 24|]);
    expect.equal(result[2].term, testInputs[1]);
    expect.array(result[2].positions).toEqual([|4, 18, 24|]);
  });

  test("Works for large test: Oni2", ({expect, _}) => {
    let testPattern = "token";

    let resultArray = fzySearchArray(TestArray.oniTestInput, testPattern);

    let bestScore = ref(0.0);
    let bestResult = ref("");
    let bestPositions = ref([||]);

    bestScore := resultArray[0].score;
    bestResult := resultArray[0].term;
    bestPositions := resultArray[0].positions;

    expect.int(Array.length(resultArray)).toBe(61);
    expect.float(bestScore^).toBeCloseTo(4.78);
    expect.string(bestResult^).toEqual("src/editor/Model/Tokenizer.re");
    expect.array(bestPositions^).toEqual([|17, 18, 19, 20, 21|]);
  });

  test("Better match is picked", ({expect, _}) => {
    let result1 = fzySearchArray([|"abcxyz"|], "abc");
    let result2 = fzySearchArray([|"abcxyz"|], "acz");

    expect.notEqual(result1, result2);
    expect.equal(result1[0].score > result2[0].score, true);
  });
});

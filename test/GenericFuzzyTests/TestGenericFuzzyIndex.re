open TestFramework;

describe("General Index: Match scores should be correct.", ({test, _}) => {
  test("Doesn't match index when not possible", ({expect}) => {
    let result = ReasonFuzz.generalIndexMatch(~line="abc", ~pattern="abx");
    expect.equal(result, None);
  });

  test("Does match when possible", ({expect}) => {
    let result = ReasonFuzz.generalIndexMatch(~line="axbycz", ~pattern="abc");
    expect.notEqual(result, None);
  });

  test("Correctly score case-match higher", ({expect}) => {
    let testString = "SRC";
    let testList = [|"browser/src/index.ts", "browser/SRC/index.ts"|];

    let result1 =
      ReasonFuzz.generalIndexMatch(~line=testList[0], ~pattern=testString);
    let result2 =
      ReasonFuzz.generalIndexMatch(~line=testList[1], ~pattern=testString);

    expect.notEqual(result1, None);
    expect.notEqual(result2, None);

    expect.notEqual(result1, result2);

    let (score1, indexes1) =
      switch (result1) {
      | Some(match) => (match.score, match.indicies)
      | None => (min_int, [||])
      };

    let (score2, indexes2) =
      switch (result2) {
      | Some(match) => (match.score, match.indicies)
      | None => (min_int, [||])
      };

    expect.equal(score2 > score1, true);
    expect.array(indexes1).toEqual([|8, 9, 10|]);
    expect.array(indexes2).toEqual([|8, 9, 10|]);
  });

  test(
    "Correctly sorts results for shortest result on file name.", ({expect}) => {
    let testPattern = "main";
    let testInputs = [|
      "packages/core/src/main.tex",
      "packages/core/test/main.tex",
      "packages/core/test/oni/main.tex",
    |];

    let bestMatch = ref("");
    let bestScore = ref(min_int);
    let bestMatchIndex = ref([||]);

    for (i in 0 to Array.length(testInputs) - 1) {
      let result =
        ReasonFuzz.generalIndexMatch(
          ~line=testInputs[i],
          ~pattern=testPattern,
        );

      let (score, indexes) =
        switch (result) {
        | Some(match) => (match.score, match.indicies)
        | None => ((-1), [||])
        };

      if (score > bestScore^) {
        bestScore := score;
        bestMatch := testInputs[i];
        bestMatchIndex := indexes;
      };
    };

    expect.equal(bestMatch^, testInputs[0]);

    expect.array(bestMatchIndex^).toEqual([|18, 19, 20, 21|]);
  });

  test("Index match is correct", ({expect}) => {
    let result = ReasonFuzz.generalIndexMatch(~line="axbycz", ~pattern="abc");

    let matches =
      switch (result) {
      | Some(match) => match.indicies
      | None => [||]
      };

    expect.array(matches).toEqual([|0, 2, 4|]);
  });

  test("Correctly sorts results for fuzzy match.", ({expect}) => {
    let testInputs = [|
      "packages/demo/src/BufferEditor.ts",
      "packages/demo/src/BufferEditorContainer.ts",
      "packages/core/src/astBackedEditing.ts",
    |];

    let testPattern = "aBE";
    let bestMatch = ref("");
    let bestScore = ref(min_int);
    let bestMatchIndex = ref([||]);

    for (i in 0 to Array.length(testInputs) - 1) {
      let result =
        ReasonFuzz.generalIndexMatch(
          ~line=testInputs[i],
          ~pattern=testPattern,
        );

      let (score, indexes) =
        switch (result) {
        | Some(match) => (match.score, match.indicies)
        | None => ((-1), [||])
        };

      if (score > bestScore^) {
        bestScore := score;
        bestMatch := testInputs[i];
        bestMatchIndex := indexes;
      };
    };

    expect.equal(bestMatch^, testInputs[2]);

    expect.array(bestMatchIndex^).toEqual([|18, 21, 27|]);
  });

  test("Works for large input", ({expect}) => {
    let bestMatch = ref("");
    let bestScore = ref(min_int);
    let bestMatchIndex = ref([||]);

    for (i in 0 to Array.length(TestArray.testInput) - 1) {
      let result =
        ReasonFuzz.generalIndexMatch(
          ~line=TestArray.testInput[i],
          ~pattern="quickOpenScore",
        );

      let (score, indexes) =
        switch (result) {
        | Some(match) => (match.score, match.indicies)
        | None => ((-1), [||])
        };

      if (score > bestScore^) {
        bestScore := score;
        bestMatch := TestArray.testInput[i];
        bestMatchIndex := indexes;
      };
    };

    expect.equal(
      bestMatch^,
      "./src/vs/base/parts/quickopen/common/quickOpenScorer.ts",
    );

    expect.array(bestMatchIndex^).toEqual([|
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

  test("Work for even larger input", ({expect}) => {
    let bestMatch = ref("");
    let bestScore = ref(min_int);
    let bestMatchIndex = ref([||]);

    for (i in 0 to Array.length(TestArray.linuxTest) - 1) {
      let result =
        ReasonFuzz.generalIndexMatch(
          ~line=TestArray.linuxTest[i],
          ~pattern="gpio-regulator",
        );

      let (score, indexes) =
        switch (result) {
        | Some(match) => (match.score, match.indicies)
        | None => ((-1), [||])
        };

      if (score > bestScore^) {
        bestScore := score;
        bestMatch := TestArray.linuxTest[i];
        bestMatchIndex := indexes;
      };
    };

    expect.equal(bestMatch^, "./drivers/regulator/gpio-regulator.c");

    expect.array(bestMatchIndex^).toEqual([|
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

  test("Better match is picked", ({expect}) => {
    let result1 =
      ReasonFuzz.generalIndexMatch(~line="abcxyz", ~pattern="abc");
    let result2 =
      ReasonFuzz.generalIndexMatch(~line="abcxyz", ~pattern="acz");

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

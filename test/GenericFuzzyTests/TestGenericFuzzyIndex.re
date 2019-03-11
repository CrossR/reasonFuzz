open TestFramework;

describe("General Index match scores should be correct.", ({test, _}) => {
  test("Doesn't match index when not possible", ({expect}) => {
    let result = ReasonFuzz.generalIndexMatch(~line="abc", ~pattern="abx");
    expect.equal(result, None);
  });

  test("Does match when possible", ({expect}) => {
    let result = ReasonFuzz.generalIndexMatch(~line="axbycz", ~pattern="abc");
    expect.notEqual(result, None);
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

  test("Work for large input", ({expect}) => {
    let bestMatch = ref("");
    let bestScore = ref(min_int);
    let bestMatchIndex = ref([||]);

    for (i in 0 to Array.length(TestArray.testInput) - 1) {
      let result =
        ReasonFuzz.generalIndexMatch(
          ~line=TestArray.testInput[i],
          ~pattern="quickOpenScore",
        );

      let (score, indicies) =
        switch (result) {
        | Some(match) => (match.score, match.indicies)
        | None => ((-1), [||])
        };

      if (score > bestScore^) {
        bestScore := score;
        bestMatch := TestArray.testInput[i];
        bestMatchIndex := indicies;
      };
    };

    expect.equal(
      bestMatch^,
      "./src/vs/base/parts/quickopen/common/quickOpenScorer.ts",
    );

    expect.array(bestMatchIndex^).toEqual([|
      20,
      21,
      22,
      30,
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

    for (i in 0 to Array.length(TestArray.linuxTest) - 1) {
      let result =
        ReasonFuzz.generalIndexMatch(
          ~line=TestArray.linuxTest[i],
          ~pattern="gpio-regulator",
        );

      let score =
        switch (result) {
        | Some(match) => match.score
        | None => (-1)
        };

      if (score > bestScore^) {
        bestScore := score;
        bestMatch := TestArray.linuxTest[i];
      };
    };

    expect.equal(bestMatch^, "./drivers/regulator/gpio-regulator.c");
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
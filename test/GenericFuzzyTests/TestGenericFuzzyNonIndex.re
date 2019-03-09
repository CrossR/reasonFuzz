open TestFramework;

describe("Fuzzy match scores should be correct", ({test, _}) => {
  test("Doesn't match when not possible", ({expect}) => {
    let result = ReasonFuzz.generalFuzzyMatch(~line="abc", ~pattern="abx");
    expect.equal(result, None);
  });

  test("Does match when possible", ({expect}) => {
    let result = ReasonFuzz.generalFuzzyMatch(~line="axbycz", ~pattern="abc");
    expect.notEqual(result, None);
  });

  test("Better match is picked", ({expect}) => {
    let result1 =
      ReasonFuzz.generalFuzzyMatch(~line="abcxyz", ~pattern="abc");
    let result2 =
      ReasonFuzz.generalFuzzyMatch(~line="abcxyz", ~pattern="acz");

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

describe("Index match scores should be correct", ({test, _}) => {
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
    expect.equal(matches, [|0, 2, 4|]);
  });

  test("Work for large input", ({expect}) => {
    let bestMatch = ref("");
    let bestScore = ref(0);

    for (i in 0 to Array.length(TestArray.testInput) - 1) {
      let result =
        ReasonFuzz.generalIndexMatch(
          ~line=TestArray.testInput[i],
          ~pattern="quickOpenScore",
        );

      let score =
        switch (result) {
        | Some(match) => match.score
        | None => (-1)
        };

      if (score > bestScore^) {
        bestMatch := TestArray.testInput[i];
      };
    };

    expect.equal(
      bestMatch^,
      "./src/vs/base/parts/quickopen/test/common/quickOpenScorer.test.ts",
    );
  });

  test("Work for even larger input", ({expect}) => {
    let bestMatch = ref("");
    let bestScore = ref(0);

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
        bestMatch := TestArray.linuxTest[i];
      };
    };

    expect.equal(bestMatch^, "./include/linux/regulator/gpio-regulator.h");
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
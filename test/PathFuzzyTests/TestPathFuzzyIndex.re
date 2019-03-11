open TestFramework;
open Generic_Fuzzy_Test;

describe("Path Index match scores should be correct.", ({test, _}) => {
  test("Doesn't match index when not possible", ({expect}) => {
    let result = ReasonFuzz.pathIndexMatch(~line="abc", ~pattern="abx");
    expect.equal(result, None);
  });

  test("Does match when possible", ({expect}) => {
    let result = ReasonFuzz.pathIndexMatch(~line="axbycz", ~pattern="abc");
    expect.notEqual(result, None);
  });

  test("Index match is correct", ({expect}) => {
    let result = ReasonFuzz.pathIndexMatch(~line="axbycz", ~pattern="abc");

    let matches =
      switch (result) {
      | Some(match) => match.indicies
      | None => [||]
      };

    expect.array(matches).toEqual([|0, 2, 4|]);
  });

  test("Works for large input", ({expect}) => {
    let bestMatch = ref("");
    let bestScore = ref(0);
    let bestMatchIndex = ref([||]);

    for (i in 0 to Array.length(TestArray.testInput) - 1) {
      let result =
        ReasonFuzz.pathIndexMatch(
          ~line=TestArray.testInput[i],
          ~pattern="quickOpenScore",
        );

      let (score, indexes) =
        switch (result) {
        | Some(match) => (match.score, match.indicies)
        | None => ((-1), [||])
        };

      if (score > bestScore^) {
        bestMatch := TestArray.testInput[i];
        bestMatchIndex := indexes;
      };
    };

    expect.equal(
      bestMatch^,
      "./src/vs/base/parts/quickopen/test/common/quickOpenScorer.test.ts",
    );

    expect.array(bestMatchIndex^).toEqual([|
      42,
      43,
      44,
      45,
      46,
      47,
      48,
      49,
      50,
      51,
      52,
      53,
      54,
      55,
    |]);
  });

  test("Work for even larger input", ({expect}) => {
    let bestMatch = ref("");
    let bestScore = ref(0);
    let bestMatchIndex = ref([||]);

    for (i in 0 to Array.length(TestArray.linuxTest) - 1) {
      let result =
        ReasonFuzz.pathIndexMatch(
          ~line=TestArray.linuxTest[i],
          ~pattern="gpio-regulator",
        );

      let (score, indexes) =
        switch (result) {
        | Some(match) => (match.score, match.indicies)
        | None => ((-1), [||])
        };

      if (score > bestScore^) {
        bestMatch := TestArray.linuxTest[i];
        bestMatchIndex := indexes;
      };
    };

    expect.equal(bestMatch^, "./include/linux/regulator/gpio-regulator.h");

    expect.array(bestMatchIndex^).toEqual([|
      26,
      27,
      28,
      29,
      30,
      31,
      32,
      33,
      34,
      35,
      36,
      37,
      38,
      39,
    |]);
  });

  test("Better match is picked", ({expect}) => {
    let result1 = ReasonFuzz.pathIndexMatch(~line="abcxyz", ~pattern="abc");
    let result2 = ReasonFuzz.pathIndexMatch(~line="abcxyz", ~pattern="acz");

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
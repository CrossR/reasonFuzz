open TestFramework;

describe("General: Match scores should be correct.", ({test, _}) => {
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

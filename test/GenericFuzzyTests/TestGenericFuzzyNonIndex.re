open TestFramework;

describe("Scores should be correct", ({test, _}) => {
  test("Doesn't match when not possible", ({expect}) => {
    let result = ReasonFuzzyMatching.Fuzzy.fuzzyMatch("abc", "abx");
    expect.equal(result, None);
  });

  test("Does match when possible", ({expect}) => {
    let result = ReasonFuzzyMatching.Fuzzy.fuzzyMatch("axbycz", "abc");
    expect.notEqual(result, None);
  });

  test("Better match is picked", ({expect}) => {
    let result1 = ReasonFuzzyMatching.Fuzzy.fuzzyMatch("abcxyz", "abc");
    let result2 = ReasonFuzzyMatching.Fuzzy.fuzzyMatch("abcxyz", "acz");

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

  test("Doesn't match index when not possible", ({expect}) => {
    let result = ReasonFuzzyMatching.Fuzzy.fuzzyIndicies("abc", "abx");
    expect.equal(result, None);
  });

  test("Does match when possible", ({expect}) => {
    let result = ReasonFuzzyMatching.Fuzzy.fuzzyIndicies("axbycz", "abc");
    expect.notEqual(result, None);
  });

  test("Index match is correct", ({expect}) => {
    let result = ReasonFuzzyMatching.Fuzzy.fuzzyIndicies("axbycz", "abc");

    let matches =
      switch (result) {
      | Some(match) => match.indicies
      | None => [||]
      };
    expect.equal(matches, [|0, 2, 4|]);
  });

  test("Better match is picked", ({expect}) => {
    let result1 = ReasonFuzzyMatching.Fuzzy.fuzzyIndicies("abcxyz", "abc");
    let result2 = ReasonFuzzyMatching.Fuzzy.fuzzyIndicies("abcxyz", "acz");

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
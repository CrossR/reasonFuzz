/*
 * Wrappers.re
 *
 * Helper wrappers, to make interfacing more easy.
 */

let _compareScores = (score1, score2, term1, term2) => {
  let score1Better = (-1);
  let score2Better = 1;
  let scoreEqual = 0;

  let finalScore = ref(scoreEqual);
  let scoreNotSet = ref(true);

  if (score1 > score2) {
    finalScore := score1Better;
    scoreNotSet := false;
  } else if (score2 > score1) {
    finalScore := score2Better;
    scoreNotSet := false;
  };

  /* Equal? Lets favour the shorter one */
  if (scoreNotSet^ && String.length(term1) != String.length(term2)) {
    if (String.length(term1) < String.length(term2)) {
      finalScore := score1Better;
      scoreNotSet := false;
    } else {
      finalScore := score2Better;
      scoreNotSet := false;
    };
  };

  /*
   * Is there anything else we should consider here?
   * For an index match, prefer compact ones?
   * For either, we could pass over counts of types of matches?
   *    Prefer camelcase etc.
   */

  finalScore^;
};

let compareScores =
    (
      scoreTuple1: (option(Types.MatchResult.t), string),
      scoreTuple2: (option(Types.MatchResult.t), string),
    ) => {
  let score1 = fst(scoreTuple1);
  let term1 = snd(scoreTuple1);
  let score2 = fst(scoreTuple2);
  let term2 = snd(scoreTuple2);

  let score1 =
    switch (score1) {
    | Some(match) => match.score
    | None => min_int
    };

  let score2 =
    switch (score2) {
    | Some(match) => match.score
    | None => min_int
    };

  _compareScores(score1, score2, term1, term2);
};

let compareIndexScores =
    (
      scoreTuple1: (option(Types.IndexMatchResult.t), string),
      scoreTuple2: (option(Types.IndexMatchResult.t), string),
    ) => {
  let score1 = fst(scoreTuple1);
  let term1 = snd(scoreTuple1);
  let score2 = fst(scoreTuple2);
  let term2 = snd(scoreTuple2);

  let score1 =
    switch (score1) {
    | Some(match) => match.score
    | None => min_int
    };

  let score2 =
    switch (score2) {
    | Some(match) => match.score
    | None => min_int
    };

  _compareScores(score1, score2, term1, term2);
};

/* Compare two matches */
type indexFuzzyMatcher =
  (~line: string, ~pattern: string) => option(Types.IndexMatchResult.t);
type fuzzyMatcher =
  (~line: string, ~pattern: string) => option(Types.MatchResult.t);

let fuzzySortArray =
    (inputs: array(string), query: string, comparer: fuzzyMatcher) => {
  let scoreArray =
    Array.map(item => (comparer(~line=item, ~pattern=query), item), inputs);
  Array.fast_sort(
    (item1, item2) => compareScores(item1, item2),
    scoreArray,
  );

  Array.map(item => snd(item), scoreArray);
};

let fuzzySortList =
    (inputs: list(string), query: string, comparer: fuzzyMatcher) => {
  let scoreList =
    List.map(item => (comparer(~line=item, ~pattern=query), item), inputs);
  let sortedList =
    List.fast_sort(
      (item1, item2) => compareScores(item1, item2),
      scoreList,
    );

  List.map(item => snd(item), sortedList);
};

let indexFuzzySortArray =
    (inputs: array(string), query: string, comparer: indexFuzzyMatcher) => {
  let scoreArray =
    Array.map(item => (comparer(~line=item, ~pattern=query), item), inputs);
  Array.fast_sort(
    (item1, item2) => compareIndexScores(item1, item2),
    scoreArray,
  );

  Array.map(item => snd(item), scoreArray);
};

let indexFuzzySortList =
    (inputs: list(string), query: string, comparer: indexFuzzyMatcher) => {
  let scoreList =
    List.map(item => (comparer(~line=item, ~pattern=query), item), inputs);
  let sortedList =
    List.fast_sort(
      (item1, item2) => compareIndexScores(item1, item2),
      scoreList,
    );

  List.map(item => snd(item), sortedList);
};

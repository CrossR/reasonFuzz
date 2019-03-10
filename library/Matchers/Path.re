/*
  Differences:
      - Allow_match doesn't use action.
 */

open Types;

/*****************************************************************************/

let fuzzyScore = (lChar, lId, lPrevChar, pChar, pId, pPrevChar) => {
  let score = ref(0);

  let lPrevCharType = CharType.charTypeOf(lPrevChar);

  if (pChar == lChar && CharType.charTypeOf(pChar) == CharType.Upper) {
    score := score^ + PathScore.default.bonusUpperMatch;
  } else {
    score := score^ + PathScore.default.penaltyCaseUnmatched;
  };

  if (lId == 0 && CharType.charTypeOf(lChar) != CharType.Upper) {
    score^;
  } else {
    if (lId == 0 && CharType.charTypeOf(lChar) == CharType.Upper) {
      score := score^ + PathScore.default.bonusCamel;
    };

    if (lPrevCharType == CharType.Separ) {
      score := score^ + PathScore.default.bonusSeparator;
    };

    if (CharType.charTypeOf(lPrevChar) == CharType.Lower
        && CharType.charTypeOf(lChar) == CharType.Upper) {
      score := score^ + PathScore.default.bonusCamel;
    };

    if (pId == 0) {
      score :=
        score^
        + max(
            lId * PathScore.default.penaltyLeading,
            PathScore.default.penaltyMaxLeading,
          );
    };

    score^;
  };
};

/*****************************************************************************/

let buildGraph = (line: string, pattern: string) => {
  let lineLen = String.length(line);
  let patternLen = String.length(pattern);

  let scores: ref(array(int)) = ref([||]);

  let previousMatchedId = ref(-1);
  let previousPatternChar: ref(option(Char.t)) = ref(None);
  let validMatch = ref(true);

  /* Initialise the match positions and inline scores. */
  let pId = ref(0);
  while (pId^ < patternLen - 1 && validMatch^) {
    let pChar = Char.lowercase_ascii(pattern.[pId^]);

    let statuses = ref([||]);
    let prevLineChar: ref(option(Char.t)) = ref(None);

    let lId = ref(0);
    while (lId^ < lineLen - 1 && validMatch^) {
      let lChar = Char.lowercase_ascii(line.[pId^]);

      if (lChar == pChar && lId^ > previousMatchedId^) {
        /* TODO:
           Score = fuzzy_score();
           statuses.append(MatchStatus Object with score)
           */
        let score = 0;
        statuses := Array.append(statuses^, [|score|]);
      };

      prevLineChar := Some(lChar);
      lId := lId^ + 1;
    };

    if (Array.length(statuses^) == 0) {
      validMatch := false;
    } else {
      previousMatchedId := statuses^[0];
      scores := Array.append(scores^, statuses^);
      previousPatternChar := Some(pChar);
    };

    pId := pId^ + 1;
  };

  pId := 1;
  while (pId^ < Array.length(scores^) && validMatch^) {
    let (firstHalf, lastHalf) = Helpers.splitArray(scores^, pId^);

    let previousRow = firstHalf[Array.length(firstHalf) - 1];
    let currentRow = lastHalf[0];
    ();
  };

  if (validMatch^ == false) {
    None;
  } else {
    Some(scores);
  };
};
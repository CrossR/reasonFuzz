/*
  Differences:
      - Allow_match doesn't use action.
 */

open Types;

/*****************************************************************************/

let fuzzyScore =
    (
      lChar: Char.t,
      lId: int,
      lPrevChar: option(Char.t),
      pChar: Char.t,
      pId: int,
      _pPrevChar: option(Char.t),
    ) => {
  let score = ref(0);

  let lPrevCharType = CharType.charTypeOf(lPrevChar);

  if (pChar == lChar && CharType.charTypeOf(Some(pChar)) == CharType.Upper) {
    score := score^ + PathScore.default.bonusUpperMatch;
  } else {
    score := score^ + PathScore.default.penaltyCaseUnmatched;
  };

  if (lId == 0 && CharType.charTypeOf(Some(lChar)) != CharType.Upper) {
    score^;
  } else {
    if (lId == 0 && CharType.charTypeOf(Some(lChar)) == CharType.Upper) {
      score := score^ + PathScore.default.bonusCamel;
    };

    if (lPrevCharType == CharType.Separ) {
      score := score^ + PathScore.default.bonusSeparator;
    };

    if (CharType.charTypeOf(lPrevChar) == CharType.Lower
        && CharType.charTypeOf(Some(lChar)) == CharType.Upper) {
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

let dealWithScoreRow =
    (
      index: int,
      current: MatchingStatus.t,
      next: MatchingStatus.t,
      prev: MatchingStatus.t,
      scoreBeforeIndex: int,
    ) =>
  if (current.index < next.index && (current.index < prev.index) == false) {
    let adjNum = next.index - current.index - 1;
    let finalScore = ref(current.finalScore + next.score);
    Console.log("      adjNum: " ++ string_of_int(adjNum));
    Console.log("      finalScore: " ++ string_of_int(finalScore^));

    if (adjNum == 0) {
      Console.log("        bonusAdj");
      finalScore := finalScore^ + PathScore.default.bonusAdjacency;
    } else {
      Console.log("        penaltyUnmatched");
      finalScore := finalScore^ + PathScore.default.penaltyUnmatched * adjNum;
    };

    Console.log("      return finalScore: " ++ string_of_int(finalScore^));

    (index, finalScore^, adjNum);
  } else {
    (prev.backRef, scoreBeforeIndex, prev.adjNum);
  };

let compareTwoScoreTuples =
    (scoreTuple1: (int, int, int), scoreTuple2: (int, int, int)) => {
  let (_, finalScore1, _) = scoreTuple1;
  let (_, finalScore2, _) = scoreTuple2;

  compare(finalScore1, finalScore2);
};

let buildGraph = (line: string, pattern: string) => {
  let lineLen = String.length(line);
  let patternLen = String.length(pattern);

  Console.log("Building scores array...");
  let scores: ref(array(array(MatchingStatus.t))) = ref([||]);

  let previousMatchedId = ref(-1);
  let pPrevChar: ref(option(Char.t)) = ref(None);
  let validMatch = ref(true);

  Console.log("Starting first while loop...");
  /* Initialise the match positions and inline scores. */
  let pId = ref(0);
  while (pId^ <= patternLen - 1 && validMatch^) {
    Console.log("  First while loop iteration starting...");
    let pChar = Char.lowercase_ascii(pattern.[pId^]);
    Console.log("  pId: " ++ string_of_int(pId^));
    Console.log("  pChar: " ++ String.make(1, pChar));

    let statuses = ref([||]);
    let lPrevChar: ref(option(Char.t)) = ref(None);

    let lId = ref(0);
    while (lId^ <= lineLen - 1 && validMatch^) {
      Console.log("    First inner while loop iteration starting...");
      let lChar = Char.lowercase_ascii(line.[lId^]);
      Console.log("    lId: " ++ string_of_int(lId^));
      Console.log("    lChar: " ++ String.make(1, lChar));

      if (lChar == pChar && lId^ > previousMatchedId^) {
        let score =
          fuzzyScore(lChar, lId^, lPrevChar^, pChar, pId^, pPrevChar^);
        Console.log("    Score here was: " ++ string_of_int(score));
        let newMatch =
          MatchingStatus.create(
            ~index=lId^,
            ~score,
            ~finalScore=score,
            ~adjNum=1,
            ~backRef=0,
          );
        statuses := Array.append(statuses^, [|newMatch|]);
      };

      lPrevChar := Some(lChar);
      lId := lId^ + 1;
      Console.log("    First inner while loop iteration ending...");
    };

    if (Array.length(statuses^) == 0) {
      Console.log("                     validMatch is now false!");
      validMatch := false;
    } else {
      previousMatchedId := statuses^[0].index;
      scores := Array.append(scores^, [|statuses^|]);
      pPrevChar := Some(pChar);
    };

    pId := pId^ + 1;

    Console.log("  First while loop iteration ending...");
  };
  Console.log("Finished first while loop...");

  Console.log("Starting second while loop...");
  pId := 1;
  while (pId^ <= Array.length(scores^) - 1 && validMatch^) {
    Console.log("  Second while loop iteration starting...");
    Console.log("  pId: " ++ string_of_int(pId^));
    let (firstHalf, lastHalf) = Helpers.splitArray(scores^, pId^);
    Console.out("  scores: ");
    Console.log(scores^);
    Console.out("  firstHalf: ");
    Console.log(firstHalf);
    Console.out("  lastHalf: ");
    Console.log(lastHalf);

    let previousRow = firstHalf[Array.length(firstHalf) - 1];
    let currentRow = lastHalf[0];
    Console.out("  previousRow: ");
    Console.log(previousRow);
    Console.out("  currentRow: ");
    Console.log(currentRow);

    Console.log(
      "  Starting inner for loop from 0 to "
      ++ string_of_int(Array.length(currentRow) - 1),
    );
    Console.log(currentRow);
    for (index in 0 to Array.length(currentRow) - 1) {
      Console.log("    Second inner for loop iteration starting...");
      let next = currentRow[index];
      let prev = index > 0 ? currentRow[index - 1] : MatchingStatus.default;

      let scoreBeforeIndex = ref(prev.finalScore - prev.score + next.score);
      scoreBeforeIndex :=
        scoreBeforeIndex^
        + PathScore.default.penaltyUnmatched
        * (next.index - prev.index);

      if (prev.adjNum == 0) {
        Console.log("    Reducing score...");
        scoreBeforeIndex :=
          scoreBeforeIndex^ - PathScore.default.bonusAdjacency;
      };
      Console.log(
        "    scoreBeforeIndex: " ++ string_of_int(scoreBeforeIndex^),
      );

      let result =
        Array.mapi(
          (index, item) =>
            dealWithScoreRow(index, item, next, prev, scoreBeforeIndex^),
          previousRow,
        );
      Array.fast_sort(compareTwoScoreTuples, result);
      /* TODO: Check this sorts the way I expect! Ie, 0 is the max score. */
      let (backRef, score, adjNum) = result[0];
      Console.log("    backRef: " ++ string_of_int(backRef));
      Console.log("    score: " ++ string_of_int(score));
      Console.log("    adjNum: " ++ string_of_int(adjNum));

      let currentStatus =
        if (index > 0 && score < scoreBeforeIndex^) {
          MatchingStatus.create(
            ~index=next.index,
            ~score=next.score,
            ~finalScore=scoreBeforeIndex^,
            ~adjNum,
            ~backRef,
          );
        } else {
          MatchingStatus.create(
            ~index=next.index,
            ~score=next.score,
            ~finalScore=score,
            ~adjNum,
            ~backRef,
          );
        };

      currentRow[index] = currentStatus;
      Console.log("    Second inner for loop iteration ending...");
    };

    pId := pId^ + 1;
    Console.log("  Second while loop iteration ending...");
  };

  Console.log("Returning score...");
  if (validMatch^ == false) {
    [||];
  } else {
    Console.log(scores^);
    scores^;
  };
};

/*****************************************************************************/

let compareMatchingStatus =
    (matchStatus1: MatchingStatus.t, matchStatus2: MatchingStatus.t) => {
  compare(matchStatus1.finalScore, matchStatus2.finalScore);
};

let getBestScore = (scoresArray: array(MatchingStatus.t)) => {
  Array.fast_sort(compareMatchingStatus, scoresArray);
  scoresArray[0].finalScore;
};

let fuzzyIndicies = (~line: string, ~pattern: string) =>
  if (String.length(pattern) == 0) {
    None;
  } else {
    let scores = buildGraph(line, pattern);

    if (scores == [||]) {
      None;
    } else {
      let finalScore = getBestScore(scores[Array.length(scores) - 1]);
      let nextCol = ref(0);
      Console.log("finalScore: " ++ string_of_int(finalScore));
      Console.log("nextCol: " ++ string_of_int(nextCol^));

      let picked = ref([||]);
      let pId = ref(Array.length(scores) - 1);
      while (pId^ >= 0) {
        let status = scores[pId^][nextCol^];
        nextCol := status.backRef;
        picked := Array.append(picked^, [|status.index|]);
        pId := pId^ - 1;
      };

      let indexes = Helpers.reverseArray(picked^);
      Console.out("indexes: ");
      Console.log(indexes);
      Some(IndexMatchResult.create(finalScore, indexes));
    };
  };

let fuzzyMatch = (~line: string, ~pattern: string) =>
  if (String.length(pattern) == 0) {
    None;
  } else {
    let scores = buildGraph(line, pattern);

    if (scores == [||]) {
      None;
    } else {
      let finalScore = getBestScore(scores[Array.length(scores) - 1]);

      Some(MatchResult.create(finalScore));
    };
  };
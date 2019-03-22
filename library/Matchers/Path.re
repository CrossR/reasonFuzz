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
  let score = ref(PathScore.default.bonusMatch);

  let lPrevCharType = CharType.charTypeOf(lPrevChar);
  let lCharRole = CharRole.charRole(lPrevChar, Some(lChar));

  if (pChar == lChar) {
    if (CharType.charTypeOf(Some(pChar)) == CharType.Upper) {
      score := score^ + PathScore.default.bonusUpperMatch;
    } else {
      score := score^ + PathScore.default.bonusCaseMatch;
    };
  } else {
    score := score^ + PathScore.default.penaltyCaseUnmatched;
  };

  if (lCharRole == CharRole.Head) {
    score := score^ + PathScore.default.bonusCamel;
  };

  if (lPrevCharType == CharType.Separ) {
    score := score^ + PathScore.default.bonusSeparator;
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

/*****************************************************************************/

let dealWithScoreRow =
    (
      index: int,
      current: MatchingStatus.t,
      next: MatchingStatus.t,
      prev: MatchingStatus.t,
      scoreBeforeIndex: int,
    ) =>
  if (current.index < next.index && current.index < prev.index == false) {
    let adjNum = next.index - current.index - 1;
    let finalScore = ref(current.finalScore + next.score);

    if (adjNum == 0) {
      finalScore := finalScore^ + PathScore.default.bonusAdjacency;
    } else {
      finalScore := finalScore^ + PathScore.default.penaltyUnmatched * adjNum;
    };

    (index, finalScore^, adjNum);
  } else {
    (prev.backRef, scoreBeforeIndex, prev.adjNum);
  };

let compareTwoScoreTuples =
    (scoreTuple1: (int, int, int), scoreTuple2: (int, int, int)) => {
  let (_, finalScore1, _) = scoreTuple1;
  let (_, finalScore2, _) = scoreTuple2;

  compare(finalScore1, finalScore2) * (-1);
};

let buildGraph = (line: string, pattern: string) => {
  let lineLen = String.length(line);
  let patternLen = String.length(pattern);

  let scores: ref(array(array(MatchingStatus.t))) = ref([||]);

  let previousMatchedId = ref(-1);
  let pPrevChar: ref(option(Char.t)) = ref(None);
  let validMatch = ref(true);

  /* Initialise the match positions and inline scores. */
  let pId = ref(0);
  while (pId^ <= patternLen - 1 && validMatch^) {
    let pChar = pattern.[pId^];

    let statuses = ref([||]);
    let lPrevChar: ref(option(Char.t)) = ref(None);

    let lId = ref(0);
    while (lId^ <= lineLen - 1 && validMatch^) {
      let lChar = line.[lId^];

      if (Char.lowercase_ascii(lChar) == Char.lowercase_ascii(pChar)
          && lId^ > previousMatchedId^) {
        let score =
          fuzzyScore(lChar, lId^, lPrevChar^, pChar, pId^, pPrevChar^);
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
    };

    if (Array.length(statuses^) == 0) {
      validMatch := false;
    } else {
      previousMatchedId := statuses^[0].index;
      scores := Array.append(scores^, [|statuses^|]);
      pPrevChar := Some(pChar);
    };

    pId := pId^ + 1;
  };

  pId := 1;
  while (pId^ <= Array.length(scores^) - 1 && validMatch^) {
    let (firstHalf, lastHalf) = Helpers.splitArray(scores^, pId^);

    let previousRow = firstHalf[Array.length(firstHalf) - 1];
    let currentRow = lastHalf[0];

    for (index in 0 to Array.length(currentRow) - 1) {
      let next = currentRow[index];
      let prev = index > 0 ? currentRow[index - 1] : MatchingStatus.default;

      let scoreBeforeIndex = ref(prev.finalScore - prev.score + next.score);
      scoreBeforeIndex :=
        scoreBeforeIndex^
        + PathScore.default.penaltyUnmatched
        * (next.index - prev.index);

      if (prev.adjNum == 0) {
        scoreBeforeIndex :=
          scoreBeforeIndex^ - PathScore.default.bonusAdjacency;
      };

      let result =
        Array.mapi(
          (index, item) =>
            dealWithScoreRow(index, item, next, prev, scoreBeforeIndex^),
          previousRow,
        );
      Array.fast_sort(compareTwoScoreTuples, result);
      let (backRef, score, adjNum) = result[0];

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
    };

    pId := pId^ + 1;
  };

  if (validMatch^ == false) {
    [||];
  } else {
    scores^;
  };
};

/*****************************************************************************/

let compareMatchingStatus =
    (matchStatus1: MatchingStatus.t, matchStatus2: MatchingStatus.t) => {
  compare(matchStatus1.finalScore, matchStatus2.finalScore) * (-1);
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

      let picked = Array.make(String.length(pattern), -1);
      let pId = ref(Array.length(scores) - 1);
      let currentIndex = ref(0);

      while (pId^ >= 0) {
        let status = scores[pId^][nextCol^];
        nextCol := status.backRef;

        picked[currentIndex^] = status.index;
        pId := pId^ - 1;
        currentIndex := currentIndex^ + 1;
      };

      let indexes =
        Helpers.reverseArray(Array.sub(picked, 0, currentIndex^));
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

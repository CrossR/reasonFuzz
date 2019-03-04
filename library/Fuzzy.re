/*

Differences:
    - Allow_match doesn't use action.

*/

let debug = false;

let consoleLog = (str: string) => {
  if (debug) {
    Console.log(str);
  };
}


/*--------------------------------------------------------------------*/

let reverseArray = (backwards: array('a)) => {
  let forwards: ref(array('a)) = ref([||]);

  for (i in Array.length(backwards) - 1 downto 0) {
    forwards := Array.append(forwards^, [|backwards[i]|])
  };

  forwards^
};

/*--------------------------------------------------------------------*/
let cheapMatches = (line: string, pattern: string) => {
    let lineIndex = ref(0);
    let patternIndex = ref(0);

    while (lineIndex^ < String.length(line) &&
           patternIndex^ < String.length(pattern)) {
        let lineChar = Char.lowercase_ascii(line.[lineIndex^]);
        let patternChar = Char.lowercase_ascii(pattern.[patternIndex^]);

        if (Char.compare(lineChar, patternChar) == 0) {
            patternIndex := patternIndex^ + 1;
        }

        lineIndex := lineIndex^ + 1;
    }

    patternIndex^ != String.length(pattern) - 1
};

/* Score building / checking -------------------------------------------*/

let allowMatch = (pChar: Char.t, lChar: Char.t) => {
    Char.lowercase_ascii(lChar) == Char.lowercase_ascii(pChar)
};

let adjustScore = (score: int, lineLen: int) => {
  score - (float_of_int(lineLen + 1) |> log |> floor |> int_of_float)
};

module Action = {

  type t =
    | Miss
    | Match;

};

module CharType = {

  type t =
    | NoChar
    | Upper
    | Separ
    | Other;

  let charTypeOf = (char: option(Char.t)) => {
      switch(char) {
      | None => NoChar
      | Some(charT) => switch(charT) {
        | 'A' .. 'Z' => Upper
        | ' ' | '_' | '-' | '/' | '\\' => Separ
        | _ => Other
        };
      };
  };

};

module CharRole = {

  type t = 
    | Tail
    | Head;

  let charRole = (prev: option(Char.t), current: option(Char.t)) => {
    switch(CharType.charTypeOf(prev), CharType.charTypeOf(current)) {
    | (CharType.NoChar, CharType.Other) | (CharType.NoChar, CharType.Upper) | (CharType.Other, CharType.Upper) | (CharType.Separ, CharType.Other) | (CharType.Separ, CharType.Upper) => Head
    | _ => Tail
    };
  };

};


let matchBonus = (pId: int, pChar: Char.t, pPrevChar: option(Char.t), lId: int, lChar: Char.t, lPrevChar: option(Char.t), lastAction: Action.t) => {
    let score = ref(10);
    let patternRole = CharRole.charRole(pPrevChar, Some(pChar));
    let lineRole = CharRole.charRole(lPrevChar, Some(lChar));

    /* Pattern is a case-insensitive prefix of word */
    if (pId == lId) {
      score := score^ + 10;
    };

    /* Case match */
    if (pChar == lChar) {
      score := score^ + 8;
    };

    /* Header match */
    if (lineRole == CharRole.Head) {
      score := score^ + 9;
    };

    /* Head match aligns */
    if (lineRole == CharRole.Head && patternRole == CharRole.Head) {
      score := score^ + 10;
    };

    /* Penalty due to match in segment */
    if (lineRole == CharRole.Tail && pId > 0 && lastAction == Action.Miss) {
      score := score^ - 30;
    };

    /* Penalty due to head pattern match in middle of word */
    if (lineRole == CharRole.Tail && patternRole == CharRole.Head) {
      score := score^ - 10;
    };

    /* Penalty due to matching first char in middle */
    if (pId == 0 && lineRole == CharRole.Tail) {
      score := score^ + 10;
    };

    score^
};

let skipPenalty = (_cId: int, char: Char.t, lastAction: Action.t) => {
  let score = ref(1);

  if (lastAction == Action.Match) {
    score := score^ + 3;
  };

  if (CharType.charTypeOf(Some(char)) == CharType.Separ) {
    score := score^ + 6;
  };

  score^
};

module Score = {

  type t = {
    lastActionMiss: ref(Action.t),
    lastActionMatch: ref(Action.t),
    missScore: ref(int),
    matchScore: ref(int)
  };

  let default: t = {
    lastActionMiss: ref(Action.Miss),
    lastActionMatch: ref(Action.Miss),
    missScore: ref(min_int),
    matchScore: ref(min_int)
  };

};

/*--------------------------------------------------------------------*/

let debugDp = (line: string, pattern: string, dp: array(array(Score.t))) => {
  let lineLen = String.length(line);
  let patternLen = String.length(pattern);

  Console.out("\t");
  for (id in 0 to lineLen - 1) {
    let ch = line.[id];
    Console.out("\t\t" ++ string_of_int(id + 1) ++ "/" ++ String.make(1, ch));
  };

  for (row in 0 to patternLen - 1) {
    Console.out("\n" ++ string_of_int(row) ++ "\t");
    for (col in 0 to lineLen - 1) {
      let cell = dp[row][col];
      let result1 = cell.lastActionMiss^ == Action.Miss ? "X" : "O";
      let result2 = cell.lastActionMatch^ == Action.Miss ? "X" : "O";
      Console.out("(" ++ string_of_int(cell.missScore^) ++ "," ++  result1 ++ ")");
      Console.out("/(" ++ string_of_int(cell.matchScore^) ++ "," ++  result2 ++ ")\t");
    };
  };
};

let buildGraph = (line: string, pattern: string, compressed: bool) => {

  let lineLen = String.length(line);
  let patternLen = String.length(pattern);
  consoleLog("Line len is " ++ string_of_int(lineLen));
  consoleLog("Pattern len is " ++ string_of_int(patternLen));

  let maxRows = compressed ? 2 : patternLen + 1;

  let dp: array(array(Score.t)) = Array.make(maxRows, Array.make(lineLen + 1, Score.default));

  dp[0][0].missScore := 0;

  for (id in 0 to lineLen - 1) {
    let char = line.[id];

    let result: Score.t = {
      missScore: ref(dp[0][id].missScore^ - skipPenalty(id, char, Action.Miss)),
      lastActionMiss: ref(Action.Miss),
      matchScore: ref(min_int),
      lastActionMatch: ref(Action.Miss),
    };

    dp[0][id + 1] = result;

  };

  consoleLog("Main loop...");
  let pPrevChar: ref(option(Char.t)) = ref(None)
  for (pId in 0 to patternLen - 1) {
    consoleLog("pId: " ++ string_of_int(pId));
    let pChar = pattern.[pId];
    consoleLog("pChar: " ++ String.make(1, pChar));

    let currentRowId = compressed ? (pId + 1) land 1 : pId + 1;
    let prevRowId = compressed ? pId land 1 : pId;

    let lPrevChar: ref(option(Char.t)) = ref(None)
    for (lId in 0 to lineLen - 1) {
      consoleLog("lId: " ++ string_of_int(lId));
      let lChar = line.[lId];
      consoleLog("lChar: " ++ String.make(1, lChar));

      if (lId < pId) {
        lPrevChar := Some(lChar);
      } else {
        consoleLog("Getting prevMiss...");
        let prevMiss = dp[currentRowId][lId];

        if (pId < patternLen) {
          prevMiss.matchScore := prevMiss.matchScore^ - skipPenalty(lId, lChar, Action.Match);
          prevMiss.missScore := prevMiss.missScore^ - skipPenalty(lId, lChar, Action.Miss);
        };

        let (missScore, lastActionMiss) = if (prevMiss.matchScore^ > prevMiss.missScore^) {
          (prevMiss.matchScore, Action.Match)
        } else {
          (prevMiss.missScore, Action.Miss)
        };

        consoleLog("Getting prevMatch...");
        let preMatch = dp[prevRowId][lId];
        let matchMatchScore = if (allowMatch(pChar, lChar)) {
          preMatch.matchScore^ + matchBonus(pId, pChar, pPrevChar^, lId, lChar, lPrevChar^, Action.Match)
        } else {
          min_int
        };

        let missMatchScore = if (allowMatch(pChar, lChar)) {
          preMatch.missScore^ + matchBonus(pId, pChar, pPrevChar^, lId, lChar, lPrevChar^, Action.Match)
        } else {
          min_int
        };

        let (matchScore, lastActionMatch) = if (matchMatchScore > missMatchScore) {
          (matchMatchScore, Action.Match)
        } else {
          (missMatchScore, Action.Miss)
        };

        consoleLog("Making result...");
        let result: Score.t = {
          missScore: missScore, /* Already a ref from the prev score */
          lastActionMiss: ref(lastActionMiss),
          matchScore: ref(matchScore),
          lastActionMatch: ref(lastActionMatch),
        };

        consoleLog("Setting result...");
        dp[currentRowId][lId + 1] = result;

        consoleLog("Updating prev char...");
        lPrevChar := Some(lChar);

        consoleLog("Inner loop iter done!");
      };

      consoleLog("Outer loop iter done!");
      pPrevChar := Some(pChar);
    };
  };

  dp

};

/*--------------------------------------------------------------------*/

let fuzzyIndiciesMatch = (line: string, pattern: string) => {

  let lineLen = String.length(line);
  let patternLen = String.length(pattern);

  consoleLog("Making graph...");
  let dp = buildGraph(line, pattern, false);
  consoleLog("Made graph...");

  if (debug) {
    debugDp(line, pattern, dp);
  };

  let indiciesReversed: ref(array(int)) = ref([||]);
  let cell = dp[patternLen][lineLen];

  let (lastAction, score) = if (cell.matchScore > cell.missScore) {
    (ref(Action.Match), cell.matchScore)
  } else {
    (ref(Action.Miss), cell.missScore)
  };

  let row = ref(patternLen);
  let col = ref(lineLen);

  consoleLog("Iterating rows/cols...");
  while (row^ > 0 && col^ > 0) {

    let cell = dp[row^][col^];
    consoleLog("R: " ++ string_of_int(row^) ++ ", C: " ++ string_of_int(col^));

    if (lastAction^ == Action.Match) {
      indiciesReversed := Array.append(indiciesReversed^, [|col^|]);
      lastAction := cell.lastActionMatch^;
      row := row^ - 1;
      col := col^ - 1;
    } else {
      lastAction := cell.lastActionMiss^;
      col := col^ - 1;
    };
  };

  consoleLog("Reversing array...");
  indiciesReversed := reverseArray(indiciesReversed^);

  Some((adjustScore(score^, lineLen), indiciesReversed^))
};

let fuzzyMatchMatch = (line: string, pattern: string) => {

  let lineLen = String.length(line);
  let patternLen = String.length(pattern);

  let dp = buildGraph(line, pattern, true);

  let cell = dp[patternLen land 1][lineLen];
  let score = max(cell.matchScore^, cell.missScore^);

  Some(adjustScore(score, lineLen))
};

/*--------------------------------------------------------------------*/

let fuzzyIndicies = (line: string, pattern: string): option((int, array(int))) => {

    switch (cheapMatches(line, pattern)) {
    | true => fuzzyIndiciesMatch(line, pattern)
    | false => None
    };

};

let fuzzyMatch = (line: string, pattern: string) => {

    switch (cheapMatches(line, pattern)) {
    | true => fuzzyMatchMatch(line, pattern)
    | false => None
    };

};

/*--------------------------------------------------------------------*/
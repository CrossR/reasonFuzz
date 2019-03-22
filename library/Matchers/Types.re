/*
 * Types.re
 */

/*--------------------------- Default Matcher ------------------------------*/

module IndexMatchResult = {
  type t = {
    score: int,
    indicies: array(int),
  };

  let create = (score: int, indicies: array(int)) => {score, indicies};
};

module MatchResult = {
  type t = {score: int};

  let create = (score: int) => {score: score};
};

let awfulScore = (-4000000000000000000);

module Action = {
  type t =
    | Miss
    | Match;
};

module CharType = {
  type t =
    | NoChar
    | Upper
    | Lower
    | Separ
    | Other;

  let charTypeOf = (char: option(Char.t)) => {
    switch (char) {
    | None => NoChar
    | Some(charT) =>
      switch (charT) {
      | 'A'..'Z' => Upper
      | 'a'..'z' => Lower
      | ' '
      | '_'
      | '-'
      | '/'
      | '\\' => Separ
      | _ => Other
      }
    };
  };
};

module CharRole = {
  type t =
    | Tail
    | Head;

  let charRole = (prev: option(Char.t), current: option(Char.t)) => {
    switch (CharType.charTypeOf(prev), CharType.charTypeOf(current)) {
    | (CharType.NoChar, CharType.Lower)
    | (CharType.NoChar, CharType.Upper)
    | (CharType.Lower, CharType.Upper)
    | (CharType.Separ, CharType.Lower)
    | (CharType.Separ, CharType.Upper) => Head
    | _ => Tail
    };
  };
};

module Score = {
  type t = {
    mutable lastActionMiss: Action.t,
    mutable lastActionMatch: Action.t,
    mutable missScore: int,
    mutable matchScore: int,
  };

  let default: t = {
    lastActionMiss: Action.Miss,
    lastActionMatch: Action.Miss,
    missScore: awfulScore,
    matchScore: awfulScore,
  };

  let getDefault = (_: int) => {
    default;
  };

  let arrayOfDefault = (_: int, ~lineLen: int) => {
    let array = Array.init(lineLen + 1, getDefault);

    array;
  };

  let matrixOfDefault = (maxRows, lineLen) => {
    let matrix = Array.init(maxRows, arrayOfDefault(~lineLen));

    matrix;
  };
};

/*------------------------------ Path Matcher ------------------------------*/

module PathScore = {
  type t = {
    bonusMatch: int,
    bonusCaseMatch: int,
    bonusUpperMatch: int,
    bonusAdjacency: int,
    bonusSeparator: int,
    bonusCamel: int,
    penaltyCaseUnmatched: int,
    penaltyLeading: int,
    penaltyMaxLeading: int,
    penaltyUnmatched: int,
  };

  let default: t = {
    bonusMatch: 4,
    bonusCaseMatch: 4,
    bonusUpperMatch: 6,
    bonusAdjacency: 10,
    bonusSeparator: 8,
    bonusCamel: 8,
    penaltyCaseUnmatched: (-1),
    penaltyLeading: (-6),
    penaltyMaxLeading: (-18),
    penaltyUnmatched: (-2),
  };
};

module MatchingStatus = {
  type t = {
    index: int,
    score: int,
    finalScore: int,
    adjNum: int,
    backRef: int,
  };

  let create = (~index, ~score, ~finalScore, ~adjNum, ~backRef) => {
    index,
    score,
    finalScore,
    adjNum,
    backRef,
  };

  let default: t = {index: 0, score: 0, finalScore: 0, adjNum: 1, backRef: 0};
};

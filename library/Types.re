/*
 * Types.re
 */

/*--------------------------- Default Matcher ------------------------------*/

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

/*------------------------------ Path Matcher ------------------------------*/
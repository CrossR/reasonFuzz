let fuzzyMatch:
  (~line: string, ~pattern: string) => option(Types.MatchResult.t);
let fuzzyIndicies:
  (~line: string, ~pattern: string) => option(Types.IndexMatchResult.t);

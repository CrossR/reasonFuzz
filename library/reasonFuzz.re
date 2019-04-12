/*
 * ReasonFuzz
 *
 * Entry point to the fuzzy matching algorithms.
 *
 * There is a general fuzzy matcher, and a path specific one.
 */

/* General fuzzy matcher that will return just a score */
let generalFuzzyMatch = Matchers.General.fuzzyMatch;

/* General fuzzy matcher that will return both a score, and the match locations. */
let generalIndexMatch = Matchers.General.fuzzyIndicies;

/* A path specific fuzzy matcher that will return just a score */
let pathFuzzyMatch = Matchers.Path.fuzzyMatch;

/* A path specific fuzzy matcher that will return both a score, and the match locations. */
let pathIndexMatch = Matchers.Path.fuzzyIndicies;

/* Wrappers */
let fuzzySortArray = Matchers.Wrappers.fuzzySortArray;
let fuzzySortList = Matchers.Wrappers.fuzzySortList;

/* Helpers */
let compareScores = Matchers.Wrappers.compareScores;

/* Types */
module MatchResult = Matchers.Types.MatchResult;
module IndexMatchResult = Matchers.Types.IndexMatchResult;

/*
 * ReasonFuzz
 *
 * Entry point to the fuzzy matching algorithms.
 *
 * There is a general fuzzy matcher, and a path specific one.
 */

/* General fuzzy matcher that will return just a score */
module GeneralMatcher = Matchers.General;

/* A path specific fuzzy matcher */
module PathMatcher = Matchers.Path;

/* Fzy-powered matcher */
module Fzy = Fzy;

/* Wrappers */
let fuzzySortArray = Matchers.Wrappers.fuzzySortArray;
let fuzzySortList = Matchers.Wrappers.fuzzySortList;

/* Helpers */
let compareScores = Matchers.Wrappers.compareScores;

/* Types */
module MatchResult = Matchers.Types.MatchResult;
module IndexMatchResult = Matchers.Types.IndexMatchResult;

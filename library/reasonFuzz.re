/*
 * ReasonFuzz
 * 
 * Entry point to the fuzzy matching algorithms.
 * 
 * There is a general fuzzy matcher, and (TODO) a path specific one.
 */

/* General fuzzy matcher that will return just a score */
let generalFuzzyMatch = Matchers.General.fuzzyMatch;

/* General fuzzy matcher that will return both a score, and the match locations. */
let generalIndexMatch = Matchers.General.fuzzyIndicies;

/* General fuzzy matcher that will return just a score */
let pathFuzzyMatch = Matchers.Path.fuzzyMatch;

/* General fuzzy matcher that will return both a score, and the match locations. */
let pathIndexMatch = Matchers.Path.fuzzyIndicies;

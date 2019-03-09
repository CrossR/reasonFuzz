# reason-fuzzy-matching

[![Build Status](https://dev.azure.com/CrossR-1/reasonFuzzyMatching/_apis/build/status/CrossR.reasonFuzzyMatching?branchName=master)](https://dev.azure.com/CrossR-1/reasonFuzzyMatching/_build/latest?definitionId=2&branchName=master)


Fuzzy matching algorithms in Reason, based on https://github.com/lotabout/fuzzy-matcher.

TODO:
 - [x] : Generic/ClangD Matcher
  - [ ] : Check performance and compare scores
 - [ ] : Path Specific matcher
  - [ ] : Check performance and compare scores

**Contains the following libraries and executables:**

```
reason-fuzzy-matching@0.0.0
│
├─test/
│   name:    RunTests.exe
│   main:    RunTests.re
│   require: reason-fuzzy-matching.lib
│
└─library/
    library name: reason-fuzzy-matching.lib
    namespace:    ReasonFuzzyMatching
```

## Developing:

```
npm install -g esy
git clone <this-repo>
esy install
esy build
```

## Running Tests:

```
# Runs the "test" command in `package.json`.
esy test
```

# ReasonFuzz

[![Build Status](https://dev.azure.com/CrossR-1/reasonFuzz/_apis/build/status/CrossR.reasonFuzz?branchName=master)](https://dev.azure.com/CrossR-1/reasonFuzz/_build/latest?definitionId=3&branchName=master)


Fuzzy matching algorithms in Reason, based on https://github.com/lotabout/fuzzy-matcher.

TODO:
 - [x] : Generic/ClangD Matcher.
    - [ ] : Check performance.
    - [ ] : Compare scores to original algorithm.
 - [X] : Path Specific matcher.
    - [ ] : Check performance.
    - [X] : Compare scores to original algorithm.

**Contains the following libraries and executables:**

```
ReasonFuzz@0.0.0
│
├─test/
│   name:    RunTests.exe
│   main:    RunTests.re
│   require: ReasonFuzz
│
└─library/
    library name: ReasonFuzz
    namespace:    ReasonFuzz
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

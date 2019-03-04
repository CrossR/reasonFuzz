# reason-fuzzy-matching


[![Build Status](https://dev.azure.com/CrossR-1/reasonFuzzyMatching/_apis/build/status/CrossR.reasonFuzzyMatching?branchName=master)](https://dev.azure.com/CrossR-1/reasonFuzzyMatching/_build/latest?definitionId=2&branchName=master)


**Contains the following libraries and executables:**

```
reason-fuzzy-matching@0.0.0
│
├─test/
│   name:    TestReasonFuzzyMatching.exe
│   main:    TestReasonFuzzyMatching.re
│   require: reason-fuzzy-matching.lib
│
├─library/
│   library name: reason-fuzzy-matching.lib
│   namespace:    ReasonFuzzyMatching
│   require:
│
└─executable/
    name:    ReasonFuzzyMatching.exe
    main:    ReasonFuzzyMatching
    require: reason-fuzzy-matching.lib
```

## Developing:

```
npm install -g esy
git clone <this-repo>
esy install
esy build
```

## Running Binary:

After building the project, you can run the main binary that is produced.

```
esy x ReasonFuzzyMatching.exe 
```

## Running Tests:

```
# Runs the "test" command in `package.json`.
esy test
```

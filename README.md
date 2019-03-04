# reason-fuzzy-matching


[![CircleCI](https://circleci.com/gh/yourgithubhandle/reason-logger/tree/master.svg?style=svg)](https://circleci.com/gh/yourgithubhandle/reason-logger/tree/master)


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

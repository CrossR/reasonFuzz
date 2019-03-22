open BenchFramework;
open Generic_Fuzzy_Test;

let setup = () => ();

let benchSingleMatch = () => {
  let _ = ReasonFuzz.generalIndexMatch(~line="axbycz", ~pattern="abc");
  ();
};

let benchBasic = () => {
  let testPattern = "main";
  let testInputs = [|
    "packages/core/src/main.tex",
    "packages/core/test/main.tex",
    "packages/core/test/oni/main.tex",
  |];

  let bestMatch = ref("");
  let bestScore = ref(min_int);
  let bestMatchIndex = ref([||]);

  for (i in 0 to Array.length(testInputs) - 1) {
    let result =
      ReasonFuzz.generalIndexMatch(~line=testInputs[i], ~pattern=testPattern);

    let (score, indexes) =
      switch (result) {
      | Some(match) => (match.score, match.indicies)
      | None => ((-1), [||])
      };

    if (score > bestScore^) {
      bestScore := score;
      bestMatch := testInputs[i];
      bestMatchIndex := indexes;
    };
  };

  ();
};

let benchVSCodeSearch = () => {
  let bestMatch = ref("");
  let bestScore = ref(min_int);
  let bestMatchIndex = ref([||]);

  for (i in 0 to Array.length(TestArray.testInput) - 1) {
    let result =
      ReasonFuzz.generalIndexMatch(
        ~line=TestArray.testInput[i],
        ~pattern="quickOpenScore",
      );

    let (score, indexes) =
      switch (result) {
      | Some(match) => (match.score, match.indicies)
      | None => ((-1), [||])
      };

    if (score > bestScore^) {
      bestScore := score;
      bestMatch := TestArray.testInput[i];
      bestMatchIndex := indexes;
    };
  };

  ();
};

let benchLinuxSearch = () => {
  let bestMatch = ref("");
  let bestScore = ref(min_int);
  let bestMatchIndex = ref([||]);

  for (i in 0 to Array.length(TestArray.linuxTest) - 1) {
    let result =
      ReasonFuzz.generalIndexMatch(
        ~line=TestArray.linuxTest[i],
        ~pattern="gpio-regulator",
      );

    let (score, indexes) =
      switch (result) {
      | Some(match) => (match.score, match.indicies)
      | None => ((-1), [||])
      };

    if (score > bestScore^) {
      bestScore := score;
      bestMatch := TestArray.linuxTest[i];
      bestMatchIndex := indexes;
    };
  };

  ();
};

let options = Reperf.Options.create(~iterations=1, ());

bench(
  ~name="General Index: Single Bench",
  ~options,
  ~setup,
  ~f=benchSingleMatch,
  (),
);
bench(
  ~name="General Index: Basic Bench",
  ~options,
  ~setup,
  ~f=benchBasic,
  (),
);
bench(
  ~name="General Index: VSCode Bench",
  ~options,
  ~setup,
  ~f=benchVSCodeSearch,
  (),
);
bench(
  ~name="General Index: Linux Bench",
  ~options,
  ~setup,
  ~f=benchLinuxSearch,
  (),
);

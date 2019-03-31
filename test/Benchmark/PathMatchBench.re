open BenchFramework;
open Generic_Fuzzy_Test;

let setup = () => ();

let benchSingleMatch = () => {
  let _ = ReasonFuzz.pathIndexMatch(~line="axbycz", ~pattern="abc");
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
      ReasonFuzz.pathIndexMatch(~line=testInputs[i], ~pattern=testPattern);

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

let benchOniSearch = () => {
  let bestMatch = ref("");
  let bestScore = ref(min_int);
  let bestMatchIndex = ref([||]);

  for (i in 0 to Array.length(TestArray.oniTestInput) - 1) {
    let result =
      ReasonFuzz.pathIndexMatch(
        ~line=TestArray.oniTestInput[i],
        ~pattern="token",
      );

    let (score, indexes) =
      switch (result) {
      | Some(match) => (match.score, match.indicies)
      | None => ((-1), [||])
      };

    if (score > bestScore^) {
      bestScore := score;
      bestMatch := TestArray.oniTestInput[i];
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
      ReasonFuzz.pathIndexMatch(
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
      ReasonFuzz.pathIndexMatch(
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
  ~name="Path Index: Single Bench",
  ~options,
  ~setup,
  ~f=benchSingleMatch,
  (),
);
bench(~name="Path Index: Basic Bench", ~options, ~setup, ~f=benchBasic, ());
bench(
  ~name="Path Index: VSCode Bench",
  ~options,
  ~setup,
  ~f=benchVSCodeSearch,
  (),
);
bench(
  ~name="Path Index: Oni2 Bench",
  ~options,
  ~setup,
  ~f=benchOniSearch,
  (),
);
bench(
  ~name="Path Index: Linux Bench",
  ~options,
  ~setup,
  ~f=benchLinuxSearch,
  (),
);

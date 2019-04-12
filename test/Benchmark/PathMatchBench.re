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
  let _ =
    ReasonFuzz.fuzzySortArray(
      TestArray.oniTestInput,
      "token",
      ReasonFuzz.pathFuzzyMatch,
    );
  ();
};

let benchVSCodeSearch = () => {
  let _ =
    ReasonFuzz.fuzzySortArray(
      TestArray.testInput,
      "quickOpenScore",
      ReasonFuzz.pathFuzzyMatch,
    );
  ();
};

let benchLinuxSearch = () => {
  let _ =
    ReasonFuzz.fuzzySortArray(
      TestArray.linuxTest,
      "gpio-regulator",
      ReasonFuzz.pathFuzzyMatch,
    );
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

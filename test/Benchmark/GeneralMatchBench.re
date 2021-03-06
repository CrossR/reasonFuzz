open BenchFramework;
open Generic_Fuzzy_Test;
open ReasonFuzz.GeneralMatcher;

let setup = () => ();

let benchSingleMatch = () => {
  let _ = fuzzyIndicies(~line="axbycz", ~pattern="abc");
  ();
};

let benchBasic = () => {
  let testPattern = "main";
  let testInputs = [|
    "packages/core/src/main.tex",
    "packages/core/test/main.tex",
    "packages/core/test/oni/main.tex",
  |];

  for (i in 0 to Array.length(testInputs) - 1) {
    let _ = fuzzyIndicies(~line=testInputs[i], ~pattern=testPattern);
    ();
  };

  ();
};

let benchVSCodeSearch = () => {
  for (i in 0 to Array.length(TestArray.vscodeInput) - 1) {
    let _ =
      fuzzyIndicies(
        ~line=TestArray.vscodeInput[i],
        ~pattern="quickOpenScore",
      );
    ();
  };

  ();
};

let benchLinuxSearch = () => {
  for (i in 0 to Array.length(TestArray.linuxTest) - 1) {
    let _ =
      fuzzyIndicies(~line=TestArray.linuxTest[i], ~pattern="gpio-regulator");
    ();
  };

  ();
};

let options = Reperf.Options.create(~iterations=1, ());

bench(
  ~name="General: Single Bench",
  ~options,
  ~setup,
  ~f=benchSingleMatch,
  (),
);
bench(~name="General: Basic Bench", ~options, ~setup, ~f=benchBasic, ());
bench(
  ~name="General: VSCode Bench",
  ~options,
  ~setup,
  ~f=benchVSCodeSearch,
  (),
);
bench(
  ~name="General: Linux Bench",
  ~options,
  ~setup,
  ~f=benchLinuxSearch,
  (),
);

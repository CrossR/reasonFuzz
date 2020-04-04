open BenchFramework;
open Generic_Fuzzy_Test;
open ReasonFuzz.Fzy;

let setup = () => ();

let benchSingleMatch = () => {
  let _ = fzySearchArray([|"axbycz"|], "abc");
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
    let _ = fzySearchArray([|testInputs[i]|], testPattern);
    ();
  };
};

let benchOniSearch = () => {
  for (i in 0 to Array.length(TestArray.oniTestInput) - 1) {
    let _ = fzySearchArray([|TestArray.oniTestInput[i]|], "token");
    ();
  };

  ();
};

let benchVSCodeSearch = () => {
  for (i in 0 to Array.length(TestArray.vscodeInput) - 1) {
    let _ = fzySearchArray([|TestArray.vscodeInput[i]|], "quickOpenScore");
    ();
  };

  ();
};

let benchLinuxSearch = () => {
  for (i in 0 to Array.length(TestArray.linuxTest) - 1) {
    let _ = fzySearchArray([|TestArray.linuxTest[i]|], "gpio-regulator");
    ();
  };

  ();
};

let options = Reperf.Options.create(~iterations=1, ());

bench(~name="FzyItem: Single Bench", ~options, ~setup, ~f=benchSingleMatch, ());
bench(~name="FzyItem: Basic Bench", ~options, ~setup, ~f=benchBasic, ());
bench(~name="FzyItem: VSCode Bench", ~options, ~setup, ~f=benchVSCodeSearch, ());
bench(~name="FzyItem: Oni2 Bench", ~options, ~setup, ~f=benchOniSearch, ());
bench(~name="FzyItem: Linux Bench", ~options, ~setup, ~f=benchLinuxSearch, ());

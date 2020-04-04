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

  let _ = fzySearchArray(testInputs, testPattern);

  ();
};

let benchOniSearch = () => {
  let _ = fzySearchArray(TestArray.oniTestInput, "token");

  ();
};

let benchVSCodeSearch = () => {
  let _ = fzySearchArray(TestArray.vscodeInput, "quickOpenScore");
  ();
};

let benchLinuxSearch = () => {
  let _ = fzySearchArray(TestArray.linuxTest, "gpio-regulator");
  ();
};

let options = Reperf.Options.create(~iterations=1, ());

bench(~name="Fzy: Single Bench", ~options, ~setup, ~f=benchSingleMatch, ());
bench(~name="Fzy: Basic Bench", ~options, ~setup, ~f=benchBasic, ());
bench(~name="Fzy: VSCode Bench", ~options, ~setup, ~f=benchVSCodeSearch, ());
bench(~name="Fzy: Oni2 Bench", ~options, ~setup, ~f=benchOniSearch, ());
bench(~name="Fzy: Linux Bench", ~options, ~setup, ~f=benchLinuxSearch, ());

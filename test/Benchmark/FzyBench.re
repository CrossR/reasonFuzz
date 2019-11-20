open BenchFramework;
open Generic_Fuzzy_Test;

let setup = () => ();

let benchSingleMatch = () => {
  let _ = Fzy.searchForItem([|"axbycz"|], "abc");
  ();
};

let benchBasic = () => {
  let testPattern = "main";
  let testInputs = [|
    "packages/core/src/main.tex",
    "packages/core/test/main.tex",
    "packages/core/test/oni/main.tex",
  |];

  let _ = Fzy.searchForItem(testInputs, testPattern);

  ();
};

let benchOniSearch = () => {
  let _ = Fzy.searchForItem(TestArray.oniTestInput, "token");

  ();
};

let benchVSCodeSearch = () => {
  let _ = Fzy.searchForItem(TestArray.testInput, "quickOpenScore");
  ();
};

let benchLinuxSearch = () => {
  let _ = Fzy.searchForItem(TestArray.linuxTest, "gpio-regulator");
  ();
};

let options = Reperf.Options.create(~iterations=1, ());

bench(~name="Fzy: Single Bench", ~options, ~setup, ~f=benchSingleMatch, ());
bench(~name="Fzy: Basic Bench", ~options, ~setup, ~f=benchBasic, ());
bench(~name="Fzy: VSCode Bench", ~options, ~setup, ~f=benchVSCodeSearch, ());
bench(~name="Fzy: Oni2 Bench", ~options, ~setup, ~f=benchOniSearch, ());
bench(~name="Fzy: Linux Bench", ~options, ~setup, ~f=benchLinuxSearch, ());

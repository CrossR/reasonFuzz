open BenchFramework;
open Generic_Fuzzy_Test;
open ReasonFuzz.Fzy;

let setup = () => ();

let oniTestInput = Array.to_list(TestArray.oniTestInput);
let vsCodeInput = Array.to_list(TestArray.vscodeInput);
let linuxTest = Array.to_list(TestArray.linuxTest);

let benchSingleMatch = () => {
  let _ = fzySearchList(["axbycz"], "abc", ());
  ();
};

let benchBasic = () => {
  let testPattern = "main";
  let testInputs = [
    "packages/core/src/main.tex",
    "packages/core/test/main.tex",
    "packages/core/test/oni/main.tex",
  ];

  let _ = fzySearchList(testInputs, testPattern, ());

  ();
};

let benchOniSearch = () => {
  let _ = fzySearchList(oniTestInput, "token", ());

  ();
};

let benchVSCodeSearch = () => {
  let _ = fzySearchList(vsCodeInput, "quickOpenScore", ());
  ();
};

let benchLinuxSearch = () => {
  let _ = fzySearchList(linuxTest, "gpio-regulator", ());
  ();
};

let options = Reperf.Options.create(~iterations=1, ());

bench(~name="Fzy [List]: Single Bench", ~options, ~setup, ~f=benchSingleMatch, ());
bench(~name="Fzy [List]: Basic Bench", ~options, ~setup, ~f=benchBasic, ());
bench(~name="Fzy [List]: VSCode Bench", ~options, ~setup, ~f=benchVSCodeSearch, ());
bench(~name="Fzy [List]: Oni2 Bench", ~options, ~setup, ~f=benchOniSearch, ());
bench(~name="Fzy [List]: Linux Bench", ~options, ~setup, ~f=benchLinuxSearch, ());

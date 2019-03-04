/*
 * Helpers.re
 */

let debug = false;

let consoleLog = (str: string) => {
  if (debug) {
    Console.log(str);
  };
}

let reverseArray = (backwards: array('a)) => {
  let forwards: ref(array('a)) = ref([||]);

  for (i in Array.length(backwards) - 1 downto 0) {
    forwards := Array.append(forwards^, [|backwards[i]|])
  };

  forwards^
};
/*
 * Helpers.re
 */

let reverseArray = (backwards: array('a)) => {
  let forwards: ref(array('a)) = ref([||]);

  for (i in Array.length(backwards) - 1 downto 0) {
    forwards := Array.append(forwards^, [|backwards[i]|]);
  };

  forwards^;
};

let splitArray = (inputArray: array('a), midPoint: int) => {
  Console.log(
    "HELPER: inputArray len: " ++ string_of_int(Array.length(inputArray)),
  );
  Console.log(
    "HELPER: Slices: (0, "
    ++ string_of_int(midPoint)
    ++ ", "
    ++ string_of_int(Array.length(inputArray) - 1)
    ++ ")",
  );

  Console.log("HELPER: Doing first half....");
  let firstHalf = Array.sub(inputArray, 0, midPoint);

  Console.log("HELPER: Doing last half....");
  let lastHalf =
    Array.sub(inputArray, midPoint - 1, Array.length(inputArray) - 1);

  Console.log("HELPER: Returning....");
  Console.log(
    "HELPER: firstHalf len: " ++ string_of_int(Array.length(firstHalf)),
  );
  Console.log(
    "HELPER: lastHalf len: " ++ string_of_int(Array.length(lastHalf)),
  );
  (firstHalf, lastHalf);
};
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
  /* Console.out("HELPER: inputArray:")
  Console.log(inputArray);

  Console.log(
    "HELPER: Slices: (0, "
    ++ string_of_int(midPoint)
    ++ ", "
    ++ string_of_int(Array.length(inputArray) - 1)
    ++ ")",
  );

  Console.log("HELPER: Doing first half...."); */
  let firstHalf = Array.sub(inputArray, 0, midPoint);

  /* Console.log("HELPER: Doing last half...."); */
  let lastHalf = ref([||]);

  if (midPoint == Array.length(inputArray) - 1) {
    lastHalf := [|inputArray[midPoint]|];
  } else {
    lastHalf := Array.sub(inputArray, midPoint, Array.length(inputArray) - 1);
  };

  /* Console.out("HELPER: firstHalf:")
  Console.log(firstHalf);
  Console.out("HELPER: lastHalf:")
  Console.log(lastHalf);

  Console.log("HELPER: Returning...."); */
  (firstHalf, lastHalf^);
};
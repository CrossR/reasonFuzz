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
  let firstHalf = Array.sub(inputArray, 0, midPoint);
  let lastHalf =
    Array.sub(inputArray, midPoint, Array.length(inputArray) - midPoint);

  (firstHalf, lastHalf);
};

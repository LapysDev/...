console.log((function(count, digits, zeroed, maximum) {
  if (0 === digits.length) return [];
  var maximum = arguments.length > 3 && maximum.length ? maximum : [digits[0]];

  // …
  while (count--) {
    var increment = true;
    var rank      = maximum.length - 1;

    // …
    for (; digits[digits.length - 1] === maximum[rank]; --rank) {
      maximum[rank] = digits[0];

      if (0 === rank) {
        increment = false;
        maximum.unshift(digits[zeroed ? 1 : 0]);

        break
      }
    }

    if (!increment)
    continue;

    for (var index = digits.length; index--; )
    if (digits[index] === maximum[rank]) {
      maximum[rank] = digits[index + 1];
      break
    }
  }

  return maximum
})(845, [..."0123456789"], true))

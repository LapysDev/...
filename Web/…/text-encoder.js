var BIT_WIDTH = 8, MAX_BYTE_SIZE = 64;
var encoding = [0n];

// ...
"Hello, World!\0".split("").forEach(function(character, position) {
  var count = encoding.length - 1;
  var index = BigInt(character.charCodeAt(0));
  var fallback = encoding[count];

  position        -= count * (BIT_WIDTH / 2);
  encoding[count] |= index << BigInt(BIT_WIDTH * position);

  if (encoding[count] >= (1n << BigInt(MAX_BYTE_SIZE))) {
    encoding[count] = fallback;
    encoding.push(index)
  }
});

encoding

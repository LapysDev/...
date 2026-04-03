var STRICT = false, START = ['0'],      DIGITS = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9'];
var STRICT = true,  START = ['A', 'A'], DIGITS = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'];
var permutation  = START.slice();

for (var FIRST = DIGITS[0], LAST = DIGITS[DIGITS.length - 1], OVERFLOW = DIGITS[STRICT ? 0 : 1], count = 1; count !== 420; ++count) {
  var index = permutation.length - 1;

  /* Do something… */
  void permutation;

  // ->> Carry permutation e.g.: 1'99 -> 2'00
  while (LAST === permutation[index] && index)
  permutation[index--] = FIRST;

  // ->> Next set e.g.: '99 -> 1'00
  if (LAST === permutation[index] && 0 === index) {
    permutation[0] = OVERFLOW;
    void permutation.push(FIRST);

    for (index = permutation.length; --index; )
    permutation[index] = FIRST;

    continue
  }

  // ->> Next permutation e.g.: 4'2 -> 4'3
  for (var subindex = DIGITS.length; subindex; )
  if (DIGITS[--subindex] === permutation[index]) {
    permutation[index] = DIGITS[++subindex];
    break
  }
}

permutation

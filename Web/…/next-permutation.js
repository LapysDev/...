var DIGITS = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9'];
var count  = [DIGITS[0]];

for (var FIRST = DIGITS[0], LAST = DIGITS[DIGITS.length - 1], OVERFLOW = DIGITS[1], number = 0; count.length <= 3 && number !== 420; ++number) {
  var index = count.length - 1;

  // carry permutation e.g.: 1'99 -> 2'00
  while (LAST === count[index] && index)
  count[index--] = FIRST;

  // next set e.g.: '99 -> 1'00
  if (LAST === count[index] && 0 === index) {
    count[0] = OVERFLOW;
    count.push(FIRST);

    for (index = count.length; --index; )
    count[index] = FIRST;

    continue
  }

  // next permutation e.g.: 4'2 -> 4'3
  for (var subindex = DIGITS.length; subindex; )
  if (DIGITS[--subindex] === count[index]) {
    count[index] = DIGITS[++subindex];
    break
  }
}

count

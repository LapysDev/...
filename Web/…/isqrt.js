function isqrt(x) {
    var approximation = 0;
    var shift = 32; // bit width of x

    shift += shift & 1; // round to nearest multiple of 2

    do {
        approximation <<= 1; // make space for next guessed bit
        approximation |= 1; // guess that the next bit is 1

        shift -= 2;
        approximation ^= approximation * approximation > (x >> shift) // revert if guess is too high
    } while (shift);

    return approximation
}

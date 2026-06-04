/* Modification > Number > To Roman Digits --- REDACT (Lapys) */
Number.toRomanDigits = function toRomanDigits(number) {
    var evaluation = "";

    if (number <= 4999) {
        var digits = ['I', 'V', 'X', 'L', 'C', 'D', 'M'];

        for (var exponent = 0; number; ++exponent) {
            var component = "";

            switch (number % 10) {
                case 0: break;
                case 1: component += digits[exponent << 1]; break;
                case 2: component += digits[exponent << 1].repeat(2); break;
                case 3: component += digits[exponent << 1].repeat(3); break;
                case 4: component += ((exponent << 1) + 1 < digits.length ?
                    digits[exponent << 1] + digits[(exponent << 1) + 1] :
                    digits[exponent << 1].repeat(4)
                ); break;
                case 5: component += digits[(exponent << 1) + 1]; break;
                case 6: component += digits[(exponent << 1) + 1] + digits[exponent << 1]; break;
                case 7: component += digits[(exponent << 1) + 1] + digits[exponent << 1].repeat(2); break;
                case 8: component += digits[(exponent << 1) + 1] + digits[exponent << 1].repeat(3); break;
                case 9: component += digits[exponent << 1] + digits[(exponent << 1) + 2]; break;
            }

            evaluation = component + evaluation;
            number /= 10;
            number |= 0;
        }
    }

    return evaluation;
};

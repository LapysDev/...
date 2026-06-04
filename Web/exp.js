function exp(x) {
    var e = 0;

    function fac(x) { return x ? x * fac(x - 1) : 1 }

    for (var k = 0; k < 100; ++k) e += (x ** k) / fac(k);

    return e
}

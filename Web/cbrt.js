function cbrt(number) {
    var approximation = 0;
    var greater = 1, lower = 1;
    var precision = 0.00000000000001;

    number < 1 ? lower = number : greater = number;
    while (precision < greater - lower) {
        approximation = (greater + lower) / 2;
        approximation * approximation * approximation > number ?
            greater = approximation :
            lower = approximation
    }

     return approximation
}

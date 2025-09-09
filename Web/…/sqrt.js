function sqrt(number) {
    var approximation;
    var precision = 0.00000001;
    var greater, lower;

    if (number < 1) { lower = number; greater = 1 }
    else { lower = 1; greater = number }

    while (precision < greater - lower) {
        approximation = (greater + lower) / 2;
        approximation * approximation > number ?
            greater = approximation :
            lower = approximation
    }

    return (greater + lower) / 2
}

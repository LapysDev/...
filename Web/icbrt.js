function icbrt(integer) {
    var evaluation = Infinity;

    for (
        var approximation = 2048; approximation < evaluation;
        approximation = ((2 * evaluation) + (integer / (evaluation * evaluation))) / 3
    ) evaluation = approximation;

    return evaluation
}

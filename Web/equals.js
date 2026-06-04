function equals(x, y) {
  return Object.is(x, y)
}

function equals(x, y) {
  if (x === 0 && y === 0) {
    try {
      Object.defineProperty(
        Object.defineProperty(
          {nil: undefined}, "nil",
          {configurable: false, enumerable: false, value: x, writable: false}
        ), "nil",
        {configurable: false, enumerable: false, value: y, writable: false}
      );
      return true
    } catch (error) {}

    return false
  }

  return x === y
}

function equals(x, y) {
  return x === 0 && y === 0 ? 1 / x === 1 / y : x === y
}

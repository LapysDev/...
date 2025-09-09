(function() {
  if ("object" === typeof performance) {
    if ("function" === typeof performance.now)
      return function now() { return performance.now() };

    else if ("object" === typeof performance.timing && "number" === typeof performance.timing.navigationStart) {
      if (
        "function" === typeof performance.clearMarks &&
        "function" === typeof performance.getEntriesByName &&
        "function" === typeof performance.mark
      ) return function now() {
        performance.clearMarks("__PERFORMANCE_NOW__");
        performance.mark("__PERFORMANCE_NOW__");

        return performance.getEntriesByName("__PERFORMANCE_NOW__")[0].startTime
      };

      else if ("function" === typeof Date.now)
        return function now() { return Date.now() - performance.timing.navigationStart };

      else
        return function now() { return new Date().getTime() - performance.timing.navigationStart }
    }
  }

  else if ("function" === typeof Date.now) {
    var offset = Date.now();
    return function now() { Date.now() - offset }
  }

  else {
    var offset = new Date().getTime();
    return function now() { new Date().getTime() - offset }
  }

  return null
})()

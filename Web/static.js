function foo() {
  var static = 42;

  function newfoo() {
    static = 69;
    return static
  };

  // ...
  foo = newfoo;
  return newfoo()
}

console.log(foo());
console.log(foo())

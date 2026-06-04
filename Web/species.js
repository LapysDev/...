class Text extends Array {
  static [Symbol.species] = String;
  method() { return this }
};
// function Text() {}
//   Object.setPrototypeOf(Text, Array);
//   Object.setPrototypeOf(Text.prototype, Array.prototype);

//   Object.defineProperty(Text, Symbol.species, {value: String});
//   Object.defineProperty(Text.prototype, "method", {value: function method() { return this }});

/* ... */
console.log((function(text) {
  console.log("Array.map(…)");
  console.log('\t' + "instanceof Array",  text instanceof Array);
  console.log('\t' + "instanceof String", text instanceof String);
  console.log('\t' + "instanceof Text",   text instanceof Text);

  return text
})(Array.prototype.map.call(new Text, function(character) { return character })));

console.log((function(text) {
  console.log("Text.method()");
  console.log('\t' + "instanceof Array",  text instanceof Array);
  console.log('\t' + "instanceof String", text instanceof String);
  console.log('\t' + "instanceof Text",   text instanceof Text);

  return text
})(Text.prototype.method.call(new Text)))

var combinations = ['0', '1'];
var count        = 9;
var filesystem   = require("node:fs");
// var source       = document.createElement("pre");

// ...
console.clear();
// document.body.innerText = "";
// document.body.appendChild(source);

for (var index = 0; count !== index++; ) {
  var combination = new Array(index).fill(combinations[0]);

  // ...
  console.log("Scanning " + index + "...");

  while (combination.filter(function(item) { return combinations[combinations.length - 1] === item }).length !== index) {
    var subindex = index - 1;

    // ...
    while (combination[subindex] === combinations[combinations.length - 1])
      --subindex;

    combination[subindex] = combinations[combinations.indexOf(combination[subindex]) + 1];

    while (index !== ++subindex)
    combination[subindex] = combinations[0];

    if (combination.filter(function(item) { return combination[0] === item }).length !== combination.length) {
      try { filesystem.appendFileSync("combinatorics.txt", combination.map(function(item) { return item + "" }).join("") + '\n'); }
      catch (error) { throw error }
    }
  }
}

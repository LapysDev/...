"ABC"  .replace(/([A-Z])([A-Z])([A-Z])/, "$&");         // --> "ABC"
"ABC"  .replace(/([A-Z])([A-Z])([A-Z])/, "$1");         // --> "A"
"ABC"  .replace(/([A-Z])([A-Z])([A-Z])/, "$2");         // --> "B"
"A,B"  .replace(/,/,                     "$`");         // --> "AAB"
"A,B"  .replace(/,/,                     "$'");         // --> "ABB"
"A,B"  .replace(/,/,                     "$$");         // --> "A$B"
"A,B"  .replace(/(?<comma>,)/,           "1$<comma>2"); // --> "A1,2B"
",ABC,".replace(/([A-Z])([A-Z])([A-Z])/, function(match, groupA, groupB, groupC, index, string) {
  void groupA, groupB, groupC; // --> "A", "B", "C"
  void index;                  // --> 1
  void match;                  // --> "ABC"
  void string;                 // --> ",ABC,"

  return ""
}) // --> ",,"

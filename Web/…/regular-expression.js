/(?:)/;                      // -->     ""
/w/;                         // -->     'w' "…w" "w…" "…w…" or /(?:w)/ or unnamed /(w)/
/^w/;                        // -->     'w'      "w…"
/w$/;                        // -->     'w' "…w"
/^w$/;                       // -->     'w'
/w|x/;                       // -->     "w" "x"
/w?/;                        // -->     /w{0,1}/ or /(?:|w)/
/w*/;                        // -->     /w{0,}/  or /(?:|w+)/
/w+/;                        // -->     /w{1,}/  or /(w|ww|www|wwww|…)/
/w{1}/;                      // -->     /w/
/w{2}/;                      // -->     /ww/
/w{3}/;                      // -->     /www/
/[0123456789]/;              // -->     /(0|1|2|3|4|5|6|7|8|9)/
/[^0123456789]/;             // --> not /(0|1|2|3|4|5|6|7|8|9)/
/[0-9]/;                     // -->     /(0|1|2|3|4|5|6|7|8|9)/
/[0\-9]/;                    // -->     /(0|-|9)/ or /[-09]/ or /[09-]/
/\d/;                        // -->     /(0|1|2|3|4|5|6|7|8|9)/
/\D/;                        // --> not /(0|1|2|3|4|5|6|7|8|9)/
/\v/;                        // -->     //
/\V/;                        // -->     /V/
/[\S\s]/;                    // ->> Any single             character --> /[\D\d]/ or /[\W\w]/
/./s;                        // ->> Any single             character
/./;                         // ->> Any single non-newline character
/\./s;                       // ->> '.'
/[`´]w*[`´]/;                // --> '`0`' '`0´' '´0`' '´0´' … or /(?:'|`)(?:|w{1,})(?:'|`)/
/([`´])w*\1/;                // --> '`0`' '´0´'
/(?<quote>[`´])w*\k<quote>/; // --> '`0`' '´0´'
/(?i:foo)bar/;               // --> "foobar" "foObar" "fOobar" "fOObar" "Foobar" "FoObar" "FOobar" "FOObar" or /(?:[Ff][Oo][Oo])bar/
/(?i-:foo)bar/;              // --> /(?:[Ff][Oo][Oo])bar/
/(?-i:foo)bar/i;             // --> /(?:foo)[Bb][Aa][Rr]/

/(\w){2}(\w)?/d.exec(",ABC,"); // --> […, indices: [[1, 4] /* match */, [2, 3] /* $1 */, [3, 4] /* $2 */]]
/(?<word>\w+)/d.exec(",ABC,"); // --> […, indices: [[1, 4] /* match */, [1, 4] /* $<word> */], groups: {word: "ABC"}]

"ABC".replace(/(\w)(\w)(\w)/, "$&");         // --> "ABC"
"ABC".replace(/(\w)(\w)(\w)/, "$1");         // --> "A"
"ABC".replace(/(\w)(\w)(\w)/, "$2");         // --> "B"
"A,B".replace(/,/,            "$`");         // --> "AAB"
"A,B".replace(/,/,            "$'");         // --> "ABB"
"A,B".replace(/,/,            "$$");         // --> "A$B"
"A,B".replace(/(?<comma>,)/,  "1$<comma>2"); // --> "A1,2B"

",ABC,".replace(/(?<comma>,)/, function(match, $1, index, string, groups) {
  void $1;     // --> ','
  void groups; // --> {comma: ','}
  void index;  // --> 0
  void match;  // --> ",ABC,"
  void string; // --> ",ABC,"

  return ""
}); // --> "ABC,"

",ABC,".replace(/\w(\w)(\w)/, function(match, $1, $2, index, string) {
  void $1, $2; // --> 'B', 'C'
  void index;  // --> 1
  void match;  // --> "ABC"
  void string; // --> ",ABC,"

  return ""
}); // --> ",,"

",ABC,".replace(/\w(?:\w)(\w)/, function(match, $1, index, string) /* ->> `?:` does not register groups */ {
  void $1;     // --> 'C'
  void index;  // --> 1
  void match;  // --> "ABC"
  void string; // --> ",ABC,"

  return ""
}); // --> ",,"

",ABC,".replace(/\w(?=\w)(\w)/, function(match, $1, index, string) /* ->> `?=` does not consume (matched) groups */ {
  void $1;     // --> 'B'
  void index;  // --> 1
  void match;  // --> "AB"
  void string; // --> ",ABC,"

  return ""
}); // --> ",,"

",ABC,".replace(/\w(?!\W)(\w)/, function(match, $1, index, string) /* ->> `?!` does not consume (unmatched) groups */ {
  void $1;     // --> 'B'
  void index;  // --> 1
  void match;  // --> "AB"
  void string; // --> ",ABC,"

  return ""
}); // --> ",,"

var DECIMAL      = /^[-+]?(?:\d+(?:\.\d*)?|\.\d+)(?:[Ee][-+]?\d+)?$/; // ->> Scientific notation (`e`) supported
var DECIMAL_ZERO = /^[-+]?(?:0+(?:\.0*)?|\.0+)(?:[Ee][-+]?\d+)?$/;    // ->> Scientific notation supports non-zero exponentiation

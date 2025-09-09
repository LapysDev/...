function charCodeAt(character) {
  var limit = 0x10FFFF;
  var code  = limit;

  // ...
  function escape(integer) {
    var source = "";

    // ...
    if (integer > 0x00FFFF)
    return (
      escape(0x0D800 + (((integer - 0x010000) >>> 10) & 0x003FF)) +
      escape(0x0DC00 + (((integer - 0x010000) >>> 00) & 0x003FF))
    );

    // ...
    while (integer || source.length < 4) {
      source     = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'][integer % 16] + source;
      integer >>>= 4
    }

    return "\\u" + source
  }

  for (var converged = false; ; ) {
    var codeCharacter = new Function("return \"" + escape(code) + '"')();

    // ...
    if (code === limit - 1) {
      if (converged) {
        code += character === new Function("return \"" + escape(limit) + '"')();
        break
      }

      converged = true
    } else converged = false;

    if (character === codeCharacter) break;
    if (character < codeCharacter) { limit = code; code >>>= 1 }
    if (character > codeCharacter) code = limit <= (code << 1) ? (code + limit) >>> 1 : (code << 1)
  }

  // ...
  return code
}

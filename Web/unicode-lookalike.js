(function arial(text) {
  return text.replace(/[\x20-\x7E]/g, function(character) {
    var code = character.charCodeAt(0);

    // ...
    if (code === 32)               return "\u2002";
    if (code === 45)               return "\u2010";
    if (code >= 65 && code <= 90)  return String.fromCodePoint(0x1D5A0 + code - 65);
    if (code >= 97 && code <= 122) return String.fromCodePoint(0x1D5BA + code - 97);
    if (code >= 48 && code <= 57)  return String.fromCodePoint(0x1D7E2 + code - 48);

    return String.fromCharCode(code + 0xFEE0);
  });
})("Hello, World! 123")

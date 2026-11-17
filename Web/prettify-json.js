var alignEnd    = 0;
var alignIndent = "";
var alignStart  = 0;
var alignWidth  = 0;
var childIndent = "";
var chunks      = [];
var depth       = 0;
var entry       = null;
var formatted   = "";
var indent      = "";
var index       = 0;
var inline      = false;
var json        = `{}`
var keys        = null;
var lineIndex   = 0;
var lines       = null;
var nestedKeys  = null;
var nestedValue = null;
var match       = null;
var probe       = null;
var stack       = [{depth: 0, inline: false, value: JSON.parse(json)}];
var value       = null;

while (0 !== stack.length) {
  entry = stack.pop();

  if ("string" === typeof entry) {
    void chunks.push(entry);
    continue
  }

  depth = entry.depth;
  value = entry.value;

  if (null === value || "object" !== typeof value) {
    void chunks.push(JSON.stringify(value));
    continue
  }

  indent      = new Array(depth + 1).join("  ");
  childIndent = indent + "  ";

  if (Array.isArray(value)) {
    if (0 === value.length) {
      void chunks.push("[]");
      continue
    }

    // ->> Keep primitive-only arrays compact unless already inside an inline object.
    if (false === entry.inline) {
      for (index = 0; index !== value.length; ++index) {
        if (null !== value[index] && "object" === typeof value[index])
        break
      }

      if (index === value.length) {
        void chunks.push(JSON.stringify(value));
        continue
      }
    }

    void chunks.push("[");
    void stack.push(entry.inline ? "]" : "\r\n" + indent + "]");

    for (index = value.length - 1; index >= 0; --index) {
      void stack.push({depth: depth + 1, inline: entry.inline, value: value[index]});
      void stack.push((0 === index ? "" : ",") + (entry.inline ? (0 === index ? "" : " ") : "\r\n" + childIndent))
    }

    continue
  }

  keys = Object.keys(value).sort();

  if (0 === keys.length) {
    void chunks.push("{}");
    continue
  }

  // ->> Only inline a single-property object when its entire value can stay compact. A nested multi-property object must retain its own lines.
  inline = 1 === keys.length;

  if (inline) {
    nestedValue = value[keys[0]];

    while (null !== nestedValue && "object" === typeof nestedValue) {
      if (Array.isArray(nestedValue)) {
        for (index = 0; index !== nestedValue.length; ++index)
        if (null !== nestedValue[index] && "object" === typeof nestedValue[index]) {
          inline = false;
          break
        }

        break
      }

      nestedKeys = Object.keys(nestedValue);

      if (0 === nestedKeys.length)
      break;

      if (1 !== nestedKeys.length) {
        inline = false;
        break
      }

      nestedValue = nestedValue[nestedKeys[0]]
    }
  }

  void chunks.push("{");
  void stack .push(inline ? "}" : "\r\n" + indent + "}");

  for (index = keys.length - 1; index >= 0; --index) {
    void stack.push({depth: depth + 1, inline: inline, value: value[keys[index]]});
    void stack.push((0 === index ? "" : ",") + (inline ? (0 === index ? "" : " ") : "\r\n" + childIndent) + JSON.stringify(keys[index]) + ": ")
  }
}

formatted = chunks.join("") + "\r\n";
lines     = formatted.split("\r\n"); // ->> Align colons within uninterrupted runs of property lines at the same depth. JSON-encoded key widths account for escaped quotes and backslashes

for (lineIndex = 0; lineIndex <= lines.length; ++lineIndex) {
  match = lineIndex !== lines.length ? /^( *)("(?:\\.|[^"\\])*"): /.exec(lines[lineIndex]) : null;

  if (null !== match && alignStart === lineIndex) {
    alignIndent = match[1];
    alignWidth  = match[2].length;

    continue
  }

  if (null !== match && alignIndent === match[1]) {
    if (alignWidth < match[2].length)
    alignWidth = match[2].length;

    continue
  }

  if (alignStart + 1 < lineIndex) {
    for (alignEnd = alignStart; alignEnd !== lineIndex; ++alignEnd) {
      probe           = /^( *)("(?:\\.|[^"\\])*"): /.exec(lines[alignEnd]);
      lines[alignEnd] = probe[1] + probe[2] + new Array(alignWidth - probe[2].length + 1).join(' ') + lines[alignEnd].slice(probe[1].length + probe[2].length)
    }
  }

  alignStart  = lineIndex;
  alignIndent = null !== match ? match[1] : "";
  alignWidth  = null !== match ? match[2].length : 0
}

formatted = lines.join("\r\n");

void console.log(formatted)
formatted

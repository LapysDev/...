"Lapys".split("").map((c, i, s) => s.join("").slice(0, i + 1).match(/[A-z]/g).length & 1 ? c.toLowerCase() : c.toUpperCase()).join("")

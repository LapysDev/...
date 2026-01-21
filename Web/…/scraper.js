var { DOMParser } = require("@xmldom/xmldom");
var filesystem    = require("fs");
var http          = require("http");
var https         = require("https");

// ...
var count = 0;
console.clear(); [
  "https://grabber.zone/comics/sonic-the-hedgehog-tangle-whisper/sonic-the-hedgehog-tangle-and-whisper-1",
  "https://grabber.zone/comics/sonic-the-hedgehog-tangle-whisper/sonic-the-hedgehog-tangle-and-whisper-2",
  "https://grabber.zone/comics/sonic-the-hedgehog-tangle-whisper/sonic-the-hedgehog-tangle-and-whisper-3",
  "https://grabber.zone/comics/sonic-the-hedgehog-tangle-whisper/sonic-the-hedgehog-tangle-and-whisper-4"
].forEach(function(uri, id) {
  var id = ++count;
  return void fetch(uri, {
    body          : null,
    cache         : "no-cache",
    credentials   : "same-origin",
    headers       : {"Content-Type": "text/html"},
    method        : "GET",
    mode          : "cors",
    redirect      : "follow",
    referrerPolicy: "no-referrer"
  }).then(async function(response) {
    var parser = new DOMParser().parseFromString(await response.text(), "text/html")
    var pages  = parser.getElementsByClassName("wp-manga-chapter-img");

    // ...
    async function download(url, path) {
      return new Promise(function(resolve, reject) {
        var protocol = URL.parse(url)?.protocol === "https:" ? https : http;

        protocol.get(url, function(response) {
          var status = response.statusCode;

          if (status >= 400)                                              return reject(new Error("HTTP response status " + status));
          if (status >= 300 && status < 400 && response.headers.location) return download(response.headers.location, path).then(resolve).catch(reject);

          // ...
          var filestream = filesystem.createWriteStream(path);

          response.pipe(filestream);
          filestream.on("error",  function(error) { filesystem.unlink(path, function() { return reject (error) }) });
          filestream.on("finish", function()      { filestream.close       (function() { return resolve(path)  }) })
        }).on("error", function(error) { reject(error) })
      })
    }

    for (var index = pages.length; index--; ) {
      var path = `Sonic IDW/Tangle & Whisper/${id}/${index < 10 ? '0' + index : index}.jpg`;
      var url  = pages.item(index).getAttribute("data-src");

      try { await download(url, path); console.log(url + "\n\t  => [PASS] " + path) }
      catch (error) { console.log(url + "\n\t  => [ERROR] " + error.message) }
    }

    // ...
    return response
  })
})

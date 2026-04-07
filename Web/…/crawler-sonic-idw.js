var { DOMParser } = require("@xmldom/xmldom");
var filesystem    = require("fs");
var http          = require("http");
var https         = require("https");

// ...
var count = 0;
console.clear(); [
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-01",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-02",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-03",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-04",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-05",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-06",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-07",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-08",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-09",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-10",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-11",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-12",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-13",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-14",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-15",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-16",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-17",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-18",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-19",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-20",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-21",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-22",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-23",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-24",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-25",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-26",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-27",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-28",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-29",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-30",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-31",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-32",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-33",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-34",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-35",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-36",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-37",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-38",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-39",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-40",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-41",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-42",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-43",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-44",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-45",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-46",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-47",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-48",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-49",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-50",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-51",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-52",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-53",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-54",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-55",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-56",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-57",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-58",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-59",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-60",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-61",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-62",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-63",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-64",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-65",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-66",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-67",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-68",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-69",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-70",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-71",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-72",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-73",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-74",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-75",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-76",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-77",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-78",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-79",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-80",
  "https://grabber.zone/comics/sonic-idw/sonic-the-hedgehog-81"
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
      var path = "C:/Users/oluwa/…/Sonic IDW/#" + (id < 10 ? '0' + id : id) + '/' + (index < 10 ? '0' + index : index) + ".jpg";
      var url  = pages.item(index).getAttribute("data-src");

      try { await download(url, path); console.log(url + "\n\t  => [PASS] " + path) }
      catch (error) { console.log(url + "\n\t  => [ERROR] " + error.message) }
    }

    // ...
    return response
  })
})

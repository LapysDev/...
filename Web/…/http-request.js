/* NOTE (Lapys) -> Try `HTMLIFrameElement`s, as well? */
var asynchronous = false;
var body         = null; // --> Blob | BufferSource | FormData | Int8Array | Document | String | URLSearchParams
var method       = "GET";
var password     = null;
var requests     = [];
var url          = "https://www.example.com";
var username     = null;

// ...
document.open();

// ... ->> Spawn multiple interfaces for HTTP requests
if ("function" === typeof fetch) {
  fetch(url, {
    body          : body,                                       // WARN (Lapys) -> Must match `Content-Type` header
    cache         : "no-cache",                                 // --> default                                                                                                                                            | force-cache         | no-cache | only-if-cached | reload
    credentials   : "same-origin",                              // --> include                                                                                                                                            | omit                | same-origin
    headers       : {"Content-Type": "text/plain" /* , ... */}, // ->> Denying CORS requests limits header types to `Accept`, `Accept-Language`, `Content-Language`, and `Content-Type: application/x-www-form-urlencoded | multipart/form-data | text/plain``
    method        : method,                                     //
    mode          : "cors",                                     // ->> Unconstrained to the CORS policy --> cors                                                                                                          | no-cors                    | same-origin
    redirect      : "follow",                                   // --> error                                                                                                                                              | follow                     | manual
    referrerPolicy: "no-referrer"                               // --> no-referrer                                                                                                                                        | no-referrer-when-downgrade | origin | origin-when-cross-origin | same-origin | strict-origin | strict-origin-when-cross-origin | unsafe-url
  }).then(function(response) {
    // document.write("<div>[Fetch API]: Fetch sent<pre>" + response + "</pre></div>");
    return response
  })
}

if ("function" === typeof XMLHttpRequest) {
  requests.push({interface: new XMLHttpRequest, type: "XML HTTP Request"});
}

if ("function" === typeof ActiveXObject) {
  var requests = [];
  var types = ["Msxml2.XMLHTTP.6.0", "Msxml2.XMLHTTP.3.0", "Msxml2.XMLHTTP", "Microsoft.XMLHTTP"];

  // ...
  for (var iterator = types.length; iterator--; ) {
    try { requests.push({interface: new ActiveXObject(types[iterator]), type: types[iterator]}) }
    catch (error) {}
  }
}

// ... ->> Make an HTTP request
for (var iterator = requests.length; iterator--; )
void function(request, requestType) {
  function onabort(event) {
    document.write("<div>[" + requestType + "]: Unexpected call to abort request</div>")
  }

  function onerror(event) {
    document.write("<div>[" + requestType + "]: Error occurred in the request transfer</div>")
  }

  function onload(event) {
    document.write("<div>[" + requestType + "]: Request successfully completed</div>")
  }

  function onloadend(event) {
    var text = request.responseText;

    // ...
    if (null === text) text = "";
    else text = "<pre>" + text.replace(/</g, "&lt;").replace(/>/g, "&gt;") + "</pre>";

    document.write("<div>[" + requestType + "]: Request sent " + text + "</div>")
  }

  function onloadstart(event) {
    document.write("<div>[" + requestType + "]: Sending request</div>")
  }

  function onprogress(event) {
    var message = "bytes transferring";

    if ("number" === typeof event.loaded) {
      message = event.loaded + ' ';

      if (event.lengthComputable && "number" === typeof event.total)
      message += "of " + event.total + ' ';

      message += "bytes transferred"
    }

    message += "&hellip;";
    document.write("<div>[" + requestType + "]: " + message + "</div>");
  }

  function ontimeout(event) {
    document.write("<div>[" + requestType + "]: Request took too long and timed out</div>")
  }

  // ...
  if ("function" === request.addEventListener) {
    request.addEventListener("abort", onabort);
    request.addEventListener("error", onerror);
    request.addEventListener("load", onload);
    request.addEventListener("loadstart", onloadstart);
    request.addEventListener("loadend", onloadend);
    request.addEventListener("progress", onprogress);
    request.addEventListener("timeout", ontimeout);

    request.open(method, url, asynchronous, username, password);
    request.setRequestHeader("Content-Type", "text/plain");
    request.send(body)
  }

  else {
    function onreadystatechange(event) {
      var DONE             = 0x4;
      var HEADERS_RECEIVED = 0x2;
      var LOADING          = 0x3;
      var OPENED           = 0x1;
      var UNSENT           = 0x0;

      switch (request.readyState) {
        case DONE: {
          if (0x0 === request.status || (request.status >= 200 && request.status < 400)) onload(event);
          else if (408 === request.status || 504 === request.status) { ontimeout(event); onerror(event) }
          else onerror(event);

          onloadend(event)
        } break;

        case HEADERS_RECEIVED: onloadstart(event); break;
        case LOADING: onprogress(event); break;

        case OPENED: case UNSENT:
          /* do nothing... */
        break;
      }
    }

    "function" !== typeof request.attachEvent ?
      request.onreadystatechange = onreadystatechange :
      request.attachEvent("onreadystatechange", onreadystatechange);

    request.open(method, url, true, username, password);
    request.setRequestHeader("Content-Type", "text/plain");
    request.send(body)
  }
}(requests[iterator].interface, requests[iterator].type);

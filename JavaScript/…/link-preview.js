var awaiting = 1;
var metadata = {description: null, thumbnail: null, title: null};
var promise  = null;
var request  = new XMLHttpRequest;

// ...
request.withCredentials    = false;
request.referrerPolicy     = "no-referrer"; // ->> Unsupported
request.onreadystatechange = function(event) {
  if (0x4 !== request.readyState)
  return; /* --> not XMLHttpRequest.DONE */

  if (request.status >= 200 && request.status < 400) {
    var document            = new DOMParser().parseFromString(request.responseText, "text/html");
    var documentCollections = [document.links, document.getElementsByTagName("meta")];
    var documentTitle       = document.title;
    var ranks               = {description: 1, thumbnail: 1.0, title: 1};

    // ...
    function sanitize(text) {
      return text.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;").replace(/"/g, "&quot;").replace(/'/g, "&#039;")
    }

    for (var documentCollectionsIndex = documentCollections.length; documentCollectionsIndex; )
    for (var documentCollection = documentCollections[--documentCollectionsIndex], documentCollectionIndex = documentCollection.length; documentCollectionIndex; ) {
      var attributes = [];
      var element    = documentCollection.item(--documentCollectionIndex);
      var subranks   = {description: 0, thumbnail: 0.0, title: 0};
      var value      = null, subvalue = null;

      // ...
      function sizeof(sizes) {
        var size = 0.0;

        // ...
        for (var index = sizes.length; index; ) {
          var subsize = sizes.item(--index);

          // ...
          if (subsize === "any")
          return 1.0;

          subsize = subsize.split('x').map(function(size) { return size >>> 0 });
          subsize = subsize[0] * subsize[1];
          size    = subsize > size ? subsize : size
        }

        return size / 9007199254740991 // --> Number.MAX_SAFE_INTEGER
      }

      switch (documentCollectionsIndex) {
        case 0: attributes = [].concat(element.rel.split(/[ \f\n\t]/));              value = element.href;    subvalue = element.sizes; break; // --> <link/>
        case 1: attributes = [element.name, element.getAttribute("property") || ""]; value = element.content; subvalue = null;          break; // --> <meta/>
        default:
      }

      attributes = attributes.map(function(attribute) { return attribute.trim() });
      value      = null !== value ? value.trim() : null;

      for (var index = attributes.length; index; )
      switch (attributes[--index]) {
        case "DC.description":      subranks.description = 2; break; // --> name
        case "description":         subranks.description = 3; break; // --> name
        case "og:description":      subranks.description = 4; break; // --> property
        case "twitter:description": subranks.description = 5; break; // --> name

        case "apple-touch-icon":          subranks.thumbnail = 9.0 + sizeof(subvalue || []); break; // --> rel
        case "apple-touch-startup-image": subranks.thumbnail = 8.0;                          break; // --> rel
        case "fluid-icon":                subranks.thumbnail = 6.0;                          break; // --> rel
        case "icon":                      subranks.thumbnail = 4.0 + sizeof(subvalue || []); break; // --> rel
        case "mask-icon":                 subranks.thumbnail = 7.0;                          break; // --> rel
        case "og:image":                  subranks.thumbnail = 11.0;                         break; // --> property
        case "shortcut icon":             subranks.thumbnail = 2.0 + sizeof(subvalue || []); break; // --> rel
        case "twitter:image":             subranks.thumbnail = 12.0;                         break; // --> name

        case "DC.title":        subranks.title = 3; break; // --> name
        case "og:title":        subranks.title = 5; break; // --> property
        case "twitter:title":   subranks.title = 4; break; // --> name
        case "tweetmeme-title": subranks.title = 2; break; // --> name

        default:
      }

      if (ranks.description <= subranks.description) { metadata.description = value; ranks.description = subranks.description }
      if (ranks.thumbnail   <= subranks.thumbnail)   { metadata.thumbnail   = value; ranks.thumbnail   = subranks.thumbnail }
      if (ranks.title       <= subranks.title)       { metadata.title       = value; ranks.title       = subranks.title }
    }

    // ...
    if (null === metadata.title)
    metadata.title = documentTitle.trim();

    if (null === metadata.thumbnail) {
      awaiting                  += 1;
      request.onreadystatechange = function(event) {
        if (0x4 !== request.readyState)
        return;

        awaiting          -= 1;
        metadata.thumbnail = request.status >= 200 && request.status < 400 ? request.responseURL : null
      };

      request.open("GET", "https://www.google.com/s2/favicons?domain=" + new URL(request.responseURL).hostname + "&sz=256", true, null, null);
      request.send(null)
    }

    // ...
    metadata.description = (metadata.description || "").length > 80 ? sanitize(metadata.description.slice(0, 79)) + "&hellip;" : null !== metadata.description ? sanitize(metadata.description) : null; // ->> More than 80 characters
    metadata.title       = sanitize(metadata.title).replace(/[\n\r]/g, "")
  }

  --awaiting
};

// request.open            ("GET", "https://www.example.com", true, null, null);
request.open            ("GET", "https://www.youtube.com/watch?v=jNQXAC9IVRw", true, null, null);
request.setRequestHeader("Access-Control-Allow-Headers", "Content-Type");
request.setRequestHeader("Access-Control-Allow-Origin",  location.href);
request.setRequestHeader("Cache-Control",                "no-cache");
request.setRequestHeader("Content-Type",                 "text/plain");
request.send            (null);

promise = setInterval(function() {
  if (0 !== awaiting)
  return;

  // ...
  console.log  (metadata);
  clearInterval(promise)
}, 0 /* --> Number.EPSILON */)

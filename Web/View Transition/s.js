var PREVIEW_INDEX_FRAMING   = -1,  PREVIEW_INDEX_UNFRAMED  = -2;
var PREVIEW_REQUEST_NOTHING = 0x0, PREVIEW_REQUEST_ONGOING = 0x1, PREVIEW_REQUEST_MOVE = 0x2;

var previewMessageType       = PREVIEW_REQUEST_NOTHING; //
var previewOngoing           = false;                   //
var previewPosition          = {x: 0.00, y: 0.00};      //
var previewPositionRequested = false;                   // --> SharedArrayBuffer
var previewSourceElement     = null;                    //
var previews                 = [];                      // --> {index: Number(Int32), ongoing: false, source: HTMLElement*, target: HTMLElement}[]

/* ... */
function move(preview, x, y) {
  if (preview.source)
  preview.source.style.cssText = "left: " + x + "px !important; top: " + y + "px !important; " + preview.source.style.cssText.replace(/\s*\b(left|top)\b[^;]*(\s*;\s*|$)/gi, "")
}

function next(_) {
  var historyPreview = null;

  // ...
  for (var index = previews.length; index--; ) {
    var preview = previews[index];

    if (preview.ongoing) {
      historyPreview = null !== historyPreview ? {} : preview;
      switch (preview.index) {
        case PREVIEW_INDEX_FRAMING: {
          preview.index                 = 0;
          preview.ongoing               = false;
          preview.source.onmousemove    = null;
          preview.source.style.cssText += "; display: none !important";
          preview.target.style.cssText += "; display: none !important"
        } break;

        default:
      }
    }
  }

  // TODO (Lapys) -> Ensure this works; See `popstate`
  if (null !== historyPreview && typeof history.pushState === "function") {
    try { history.pushState({previewIndex: index}, "", location.href) }
    catch (error) { /* --> DataCloneError | DOMException | SecurityError */ }
  }
}

function previous(_) {
  for (var index = previews.length; index--; ) {
    var preview = previews[index];

    if (preview.ongoing)
    switch (preview.index) {
      case PREVIEW_INDEX_FRAMING: {
        preview.destination                .removeAttribute("data-:preview");
        preview.destination                .removeAttribute("data-:preview-destination");
        void preview.destination.parentNode.removeChild    (preview.destination);
        void preview.root                  .insertBefore   (preview.target, preview.source);
        preview.root                       .removeAttribute("data-:preview-container");
        void preview.source.parentNode     .removeChild    (preview.source);
        preview.source                     .removeAttribute("data-:preview-source");
        preview.target                     .setAttribute   ("data-:preview", "");

        preview.destination.className = preview.destination.className.replace(/\s*\bpreview(-destination)?\b\s*/g, "");
        preview.index                 = PREVIEW_INDEX_UNFRAMED;
        preview.ongoing               = false;
        preview.source.className      = preview.source.className.replace(/\s*\bpreview-source\b\s*/g, "");
        preview.source.onmousemove    = null;
        preview.target.style.cssText  = preview.target.style.cssText.replace(/\s*\b(height|width): \d+px !important(\s*;\s*|$)/gi, "")
      } break;

      default: {}
    }
  }
}

/* ... */
document.oncontextmenu = function(mouseEvent) { var mouseEvent = arguments.length ? mouseEvent : event; previous(mouseEvent) };
document.onkeydown     = function(keyEvent)   { var keyEvent   = arguments.length ? keyEvent   : event; switch (keyEvent.keyCode) { case 0xE5: case /* --> VK_ESCAPE */ 0x1B: previous(keyEvent) } };
document.onmouseup     = function(mouseEvent) { var mouseEvent = arguments.length ? mouseEvent : event; void setTimeout(next, 0.0e3, mouseEvent) };
onblur                 = function(blurEvent)  { var blurEvent  = arguments.length ? blurEvent  : event; previous(blurEvent) };
onmessage              = function(messageEvent) {
  var messageEvent = arguments.length ? messageEvent : event;

  if (PREVIEW_REQUEST_NOTHING !== previewMessageType) {
    switch (previewMessageType) {
      case PREVIEW_REQUEST_MOVE: {
        var positions = messageEvent.data.split(':');

        // ...
        previewMessageType = PREVIEW_REQUEST_NOTHING;
        move(+positions[0] - (previewSourceElement.offsetWidth / 2.0), +positions[1] - (previewSourceElement.offsetHeight / 2.0))
      } break;

      case PREVIEW_REQUEST_ONGOING: {
        var linkURL          = messageEvent.data;
        var previewNextIndex = (previewIndex + 1) % previewElements.length;

        // ...
        previewSourceElement = previewElements[previewIndex];
        // previewSourceElement.height                  = previewSourceElement.offsetHeight;
        // previewSourceElement.width                   = previewSourceElement.offsetWidth;

        void document.body               .insertBefore   (previewElements[previewNextIndex], previewSourceElement);
        previewSourceElement             .removeAttribute("data-:preview");
        previewSourceElement             .setAttribute   ("data-:preview-source", "");
        previewElements[previewNextIndex].setAttribute   ("data-:preview",        "");

        previewElements[previewNextIndex].className += " preview";
        previewElements[previewNextIndex].id         = "main";
        previewElements[previewNextIndex].src        = linkURL;
        previewSourceElement.className              += " preview-source";
        previewIndex                                 = previewNextIndex;
        previewMessageType                           = PREVIEW_REQUEST_MOVE; // ->> Does not trigger `previewPositionRequested`
        previewOngoing                               = true
      } break;
    }
  } else switch (messageEvent.data >>> 0) {
    case PREVIEW_REQUEST_ONGOING:
    case PREVIEW_REQUEST_MOVE:
      previewMessageType = messageEvent.data >>> 0
  }
};

onpopstate = function(popstateEvent) {
  var popstateEvent = arguments.length ? popstateEvent : event;
  console.log(popstateEvent)
};

if (self !== top && typeof parent.postMessage === "function") {
  // document.onmousemove = function(mouseEvent) {
  //   var mouseEvent = arguments.length ? mouseEvent : event;

  //   if (previewOngoing) {
  //     previewPosition.x = mouseEvent.clientX;
  //     previewPosition.y = mouseEvent.clientY;

  //     if (!previewPositionRequested) {
  //       previewPositionRequested = true;
  //       parent.postMessage(PREVIEW_REQUEST_MOVE, '*', [])
  //     }
  //   }
  // };

  // void setInterval(function() {
  //   if (previewPositionRequested) {
  //     previewPositionRequested = false;
  //     parent.postMessage(previewPosition.x + ':' + previewPosition.y, '*', [])
  //   }
  // }, 0.0e3)
}

for (var index = document.links.length; index--; )
document.links.item(index).ondragstart = function(mouseEvent) {
  var mouseEvent   = arguments.length ? mouseEvent : event;
  var linkElement  = mouseEvent.target || mouseEvent.srcElement;
  var linkOffset   = {x: 0.00, y: 0.00};
  var linkURL      = linkElement.href;
  var scrollOffset = {
    x: (typeof scrollX === "number" ? scrollX : typeof pageXOffset === "number" ? pageXOffset : document.documentElement.scrollLeft || document.body.scrollLeft),
    y: (typeof scrollY === "number" ? scrollY : typeof pageYOffset === "number" ? pageYOffset : document.documentElement.scrollTop  || document.body.scrollTop)
  };

  // ...
  if (typeof linkElement.getBoundingClientRect === "function") {
    var linkBounds = linkElement.getBoundingClientRect();

    linkOffset.x = linkBounds.x;
    linkOffset.y = linkBounds.y
  }

  else {
    for (var node = linkElement;            null !== node;                           node = node.offsetParent) { linkOffset.x += node.offsetLeft;         linkOffset.y += node.offsetTop }
    for (var node = linkElement.parentNode; null !== node && node.nodeType === 0x01; node = node.parentNode)   { linkOffset.x -= node.scrollLeft || 0.00; linkOffset.y -= node.scrollTop || 0.00 }
  }

  // ...
  if (self === top) /* ->> First preview (otherwise possibly hits `<iframe>` depth/ traversal limit) */ {
    for (var previewElement = linkElement; null !== previewElement && previewElement.nodeType === 0x01; previewElement = previewElement.parentNode)
    if (null !== previewElement.getAttribute("data-:preview")) {
      var preview           = null;
      var previewParentNode = previewElement.parentNode;

      // ...
      for (var index = previews.length; ; ) {
        if (--index === -1) { preview = previews[previews.push({destination: document.createElement("div"), frames: [document.createElement("iframe"), document.createElement("iframe")], index: PREVIEW_INDEX_UNFRAMED, ongoing: false, root: null, source: document.createElement("div"), target: previewElement}) - 1]; break }
        if (previewElement === previews[index].target) { preview = previews[index]; break }
      }

      if (PREVIEW_INDEX_UNFRAMED === preview.index) {
        preview.destination.className = (preview.destination.className ? ' ' : "") + "preview preview-destination";
        preview.frames[0].src         = linkURL;
        preview.index                 = PREVIEW_INDEX_FRAMING;
        preview.ongoing               = true;
        preview.root                  = previewParentNode;
        preview.source.className      = "preview-source";
        preview.source.onmousemove    = function follow(mouseEvent) { var mouseEvent = arguments.length ? mouseEvent : event; for (var index = previews.length; index--; ) { if (previews[index].source === (mouseEvent.target || mouseEvent.srcElement)) return void move(previews[index], mouseEvent.clientX - (previews[index].source.offsetWidth / 2.0), mouseEvent.clientY - (previews[index].source.offsetHeight / 2.0)) } };
        preview.source.style.cssText  = preview.source.style.cssText.replace(/\s*\bdisplay: none !important(\s*;\s*|$)/gi, "");
        preview.target.style.cssText += "; height: " + preview.target.offsetHeight + "px !important; width: " + preview.target.offsetWidth + "px !important";

        try {
          preview.destination.setAttribute   ("data-:preview",             "");
          preview.destination.setAttribute   ("data-:preview-destination", "");
          void preview.root  .insertBefore   (preview.source,      preview.target).appendChild(preview.target);
          void preview.root  .insertBefore   (preview.destination, preview.source).appendChild(preview.frames[0]);
          preview.source     .setAttribute   ("data-:preview-source", "");
          preview.target     .removeAttribute("data-:preview");
          move(preview, mouseEvent.clientX - (preview.source.offsetWidth / 2.0), mouseEvent.clientY - (preview.source.offsetHeight / 2.0));

          if (preview.root.nodeType === 0x01)
          preview.root.setAttribute("data-:preview-container", ""); // ->> Hide scrollbars when `[data-:preview-source]` intersects either edge

          if      (/function|object|unknown/.test(typeof linkElement.scrollIntoView)) linkElement.scrollIntoView({"behavior": "smooth", "block": "start", "container": "nearest", "inline": "start", valueOf: function() { return true }}); // ->> Consider further adjustments via `preview.source.scrollBy(…)`
          else if (/function|object|unknown/.test(typeof preview.source.scrollTo))    preview.source.scrollTo(linkOffset.x + scrollOffset.x, linkOffset.y + scrollOffset.y)                                                                 // ->> Check if needed initially
        } catch (error) /* --> HierarchyRequestError */ { previous(null) }

        // ... ->> Avoid consuming `VK_ESCAPE` action in Internet Explorer
        return false
      } else console.error("Unexpected transition from already transitioned view")
    }
  }

  else if (typeof parent.postMessage === "function") /* ->> Umpteenth preview */ {
    previewOngoing = true;

    document.body.setAttribute("data-:preview-container", "");
    parent.postMessage(PREVIEW_REQUEST_ONGOING,                       '*', []);
    parent.postMessage(linkURL,                                       '*', []);
    parent.postMessage(mouseEvent.clientX + ':' + mouseEvent.clientY, '*', [])
  }

  else
    console.error("Unable to transition view")
}

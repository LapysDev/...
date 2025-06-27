var { MarkdownView, Notice,      Plugin }     = require("obsidian");
var { EditorView,   PluginValue, ViewPlugin } = require("@codemirror/view");
var { requestUrl }                            = require("obsidian");

/* CITE (Lapys) -> https://github.com/dhamaniasad/obsidian-rich-links; https://docs.obsidian.md */
// --> npm i && npm audit fix --force & npm run dev
export default class LinkPreviewPlugin extends Plugin {
  static SETTINGS  = {};
  static CLICKABLE = ViewPlugin.fromClass(class ClickableButtonPlugin implements PluginValue {
    handler;

    /* ... */
    constructor(public view: EditorView) {
      this.handler = this.onMouseDown.bind(this);
      this.view.dom.addEventListener("mousedown", this.handler)
    }

    /* ... */
    destroy()           { this.view.dom.removeEventListener("mousedown", this.handler) }
    update (viewUpdate) {}

    onMouseDown(event) {
      if (LinkPreviewPlugin.removePreview(LinkPreviewPlugin.getAncestorElementByClassName(LinkPreviewPlugin.getAncestorElementByClassName(event.target, "link-preview-close"), "link-preview"), this.view)) {
        event.preventDefault ();
        event.stopPropagation()
      }
    }
  });

  /* ... */
  settings; // --> LinkPreviewPlugin

  /* ... */
  static getAncestorElementByClassName(element, className) {
    for (var ancestorElement = element; null !== ancestorElement && ancestorElement.nodeType === 0x1; ancestorElement = ancestorElement.parentNode) {
      if (ancestorElement.className.split(/\s+/).indexOf(className) !== -1)
      return ancestorElement
    }

    return null
  }

  static getLastChildElementByClassName(element, className) {
    var children = element.childNodes;

    // ...
    for (var index = children.length; index; ) {
      var childElement = children.item(--index);

      if (childElement.nodeType === 0x1 && childElement.className.split(/\s+/).indexOf(className) !== -1)
      return childElement
    }

    return null
  }

  static getPreviewTitle(preview) { return preview.getElementsByClassName("link-preview-title").item(0) }
  static isLink         (text)    { return /^(http:\/\/www\.|https:\/\/www\.|http:\/\/|https:\/\/)?[a-z0-9]+([\-.]{1}[a-z0-9]+)*\.[a-z]{2,5}(:[0-9]{1,5})?(\/.*)?$/.test(text) }

  async loadSettings() {
    var plugin = this;
    plugin.settings = Object.assign({}, LinkPreviewPlugin.SETTINGS, await plugin.loadData())
  }

  async onload() {
    var plugin = this;

    // ...
    await plugin.loadSettings();

    plugin.addCommand                   ({editorCheckCallback: function(checking, editor) { if (false === checking) { LinkPreviewPlugin.setPreviewByEditorSelectedLink(editor) } return true }, id: "preview-link", name: "Preview Link"});
    plugin.addRibbonIcon                ("link", "Preview Link", function() { if (null !== plugin.app.workspace.getActiveViewOfType(MarkdownView)) LinkPreviewPlugin.setPreviewByEditorSelectedLink(plugin.app.workspace.activeLeaf?.view.editor) });
    plugin.registerEditorExtension      (LinkPreviewPlugin.CLICKABLE);
    plugin.registerMarkdownPostProcessor(function(element, context) {
      var previewCloses = element.getElementsByClassName("link-preview-close");

      // ...
      for (var index = previewCloses.length; index; )
      previewCloses.item(--index).addEventListener("click", async function closePreviewByEvent(event) {
        var preview    = LinkPreviewPlugin.getAncestorElementByClassName(this, "link-preview");
        var pluginLeaf = plugin.app.workspace.activeLeaf;

        // ...
        if (null !== pluginLeaf && null !== preview) {
          var pluginView      = plugin.app.workspace.getActiveViewOfType(MarkdownView)?.editor.cm;
          var pluginViewState = pluginLeaf.getViewState();

          // ...
          pluginViewState.state.mode   = "source";
          pluginViewState.state.source = false;
          await pluginLeaf.setViewState(pluginViewState);

          if (LinkPreviewPlugin.removePreview(plugin.app.workspace.getActiveViewOfType(MarkdownView)?.editor.cm.dom.querySelector(".link-preview[id=\"" + preview.id + "\"]"), pluginView)) {
            event.preventDefault ();
            event.stopPropagation()
          }

          pluginViewState.state.mode   = "preview";
          pluginViewState.state.source = false;
          await pluginLeaf.setViewState(pluginViewState)
        }
      })
    });
  }

  onunload() {}

  static removePreview(preview, view) {
    if (null !== preview) {
      var previewBeginPosition = view.posAtDOM(preview, 0);
      var previewEndPosition   = view.posAtDOM(LinkPreviewPlugin.getLastChildElementByClassName(preview, "link-preview-end") || preview, 0);
      var previewTitle         = (LinkPreviewPlugin.getPreviewTitle(preview) || {innerText: null}).innerText;

      // ...
      if (null !== previewBeginPosition && null !== previewEndPosition) {
        previewBeginPosition = view.state.doc.lineAt(previewBeginPosition).from;
        previewEndPosition   = view.state.doc.lineAt(previewEndPosition)  .to;

        new Notice   ("Removed preview" + (null !== previewTitle ? " “" + previewTitle.replace(/\s/g, ' ').replace(/[“”]/g, '"') + '”' : ""));
        console.info ("Removed preview “" + (previewTitle || "").replace(/\s/g, ' ').replace(/[“”]/g, '"') + "” at " + previewBeginPosition + "–" + previewEndPosition);
        view.dispatch({changes: {from: previewBeginPosition, insert: "", to: previewEndPosition}, selection: {anchor: previewBeginPosition}, scrollIntoView: true});

        return true
      }
    }

    return false
  }

  async saveSettings() {
    var plugin = this;
    await plugin.saveData(plugin.settings)
  }

  static async setPreviewByEditorSelectedLink(editor) {
    var DEFAULT_RESPONSE_HEADERS = {"Access-Control-Allow-Headers": "Access-Control-Allow-Origin, Cache-Control, Content-Type", "Access-Control-Allow-Origin": location.href, "Cache-Control": "no-cache", "Content-Type": "text/plain"};

    var cursorPosition                = editor.getCursor();
    var document                      = null;
    var documentCollections           = [];
    var documentTitle                 = "";
    var editorSelection               = editor.somethingSelected() ? editor.getSelection().trim() : "";
    var editorSelectionCursorPosition = cursorPosition;
    var link                          = null;
    var metadata                      = {description: null, thumbnail: null, title: null};
    var metadataLink                  = null;
    var preview                       = null;
    var ranks                         = {description: 1, thumbnail: 1.0, title: 1};
    var response                      = null;
    var responseHeaders               = Object.assign({}, DEFAULT_RESPONSE_HEADERS);
    var special                       = {data: null, method: null, platform: ""};

    /* ... */
    function sanitize(text) { return text.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;").replace(/"/g, "&quot;").replace(/'/g, "&#039;") }

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

    /* ... ->> Ensure the `editor` selection is a link */
    if (false === LinkPreviewPlugin.isLink(editorSelection)) {
      new Notice  ("Select a link to turn into a preview");
      console.info((editor.somethingSelected() ? "Expected" : "Pending") + " link to preview");

      return
    }

    try { link = metadataLink = new URL(editorSelection) }
    catch (error) {
      new Notice   ("Make sure the link is correct?");
      console.error("Unexpected link `" + editorSelection.replace(/`/g, "%60") + "` encountered");

      return
    }

    // ... ->> Get “special” `metadata` about the `link`
    if (null !== metadataLink.hostname.match(/(|old\.|www\.)reddit\.com/)) {
      var redditClientID     = "";
      var redditClientSecret = "";
      var redditPassword     = "";
      var redditPostID       = (metadataLink.pathname.match(/\/comments\/([a-z0-9]+)(?:\/|$)/) || [, null])[1];
      var redditUserAgent    = "";
      var redditUsername     = "";
      var redditResponse     = await requestUrl({body: /* --> URLSearchParams */ "grant_type=password&username=" + encodeURIComponent(redditUsername) + "&password=" + encodeURIComponent(redditPassword), headers: {"Authorization": "Basic " + btoa(redditClientID + ':' + redditClientSecret), "Content-Type": "application/x-www-form-urlencoded", "User-Agent": redditUserAgent}, method: "POST", "throw": false, url: "https://www.reddit.com/api/v1/access_token"});

      // ...
      special.data     = redditPostID;
      special.platform = "Reddit";

      if (redditResponse.status === 200 && typeof redditResponse.json.access_token === "string") /* ->> Avoid `.json == {error: "invalid_grant"}` */ {
        metadataLink                                    = new URL("https://oauth.reddit.com/comments/" + redditPostID + "/.json?raw_json=1");
        responseHeaders["Access-Control-Allow-Headers"] = (responseHeaders["Access-Control-Allow-Headers"] || []).split(/\s*,\s*/).concat(["Authorization", "User-Agent"]).join(", ");
        responseHeaders["Authorization"]                = (redditResponse.json.token_type || "bearer ") + ' ' + redditResponse.json.access_token;
        responseHeaders["User-Agent"]                   = redditUserAgent;
        special.method                                  = "reddit-oauth"
      }

      else {
        metadataLink   = new URL(metadataLink.href + ".json");
        special.method = "reddit-json"
      }
    }

    // ... ->> Get `metadata` about the `link`
    for (var requesting = true; requesting; ) {
      requesting = false;
      response   = await requestUrl({contentType: "text/plain", headers: responseHeaders["Content-Type"], method: "GET", "throw": /* ->> Status code 400+ */ false, url: metadataLink.href}); // ->> `fetch(…)` without CORS restrictions

      // ... ->> Failed to fetch `link`
      if (response.status < 200 || response.status >= 300 || (function(special) {
        switch (special.method) {
          case "reddit-json":
          case "reddit-oauth": try { void response.json[0].data.children[0].data; return false } catch (error) {} return true; // ->> Assert JSON layout
          default:
        }

        return false
      })(special)) {
        console.error("Unable to preview link `" + metadataLink.href.replace(/`/g, "%60") + "`; Status " + response.status);

        // ...
        if (link === metadataLink)
          new Notice("Unable to preview link `" + metadataLink.href.replace(/`/g, "%60") + "`; Status " + response.status);

        else {
          metadataLink     = link;
          requesting       = true; // --> continue
          responseHeaders  = Object.assign({}, DEFAULT_RESPONSE_HEADERS);
          special.method   = null
        }
      }
    }

    switch (special.method) {
      case "reddit-json":
      case "reddit-oauth": {
        var post = response.json[0].data.children[0].data;

        // ...
        metadata.description = post.selftext;
        metadata.title       = post.title;

        try { metadata.thumbnail = "preview" in post && 0 !== (post.preview.images || []).length ? post.preview.images[0].source.url : new URL(post.thumbnail).href }
        catch (error) { metadata.thumbnail = special.method === "reddit-oauth" ? "https://share.redd.it/preview/post/" + special.data : null } // ->> still requires Open Authorization

        console.info("Previewing “special” " + special.platform + " link")
      } break;

      default: {
        document      = new DOMParser().parseFromString(response.text, "text/html");
        documentTitle = document.title;

        documentCollections.push(document.links, document.getElementsByTagName("meta"));

        // ... ->> Parse `metadata` from `document` `<link>` and `<meta>`
        for (var documentCollectionsIndex = documentCollections.length; documentCollectionsIndex; )
        for (var documentCollection = documentCollections[--documentCollectionsIndex], documentCollectionIndex = documentCollection.length; documentCollectionIndex; ) {
          var attributes = [];
          var element    = documentCollection.item(--documentCollectionIndex);
          var subranks   = {description: 0, thumbnail: 0.0, title: 0};
          var value      = null, subvalue = null;

          // ...
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

          if ((value || "").replace(/\s+/g, "").length && ranks.description <= subranks.description) { metadata.description = value; ranks.description = subranks.description }
          if ((value || "").replace(/\s+/g, "").length && ranks.thumbnail   <= subranks.thumbnail)   { metadata.thumbnail   = value; ranks.thumbnail   = subranks.thumbnail }
          if ((value || "").replace(/\s+/g, "").length && ranks.title       <= subranks.title)       { metadata.title       = value; ranks.title       = subranks.title }
        }
      }
    }

    // ... ->> Fallback `metadata` from link `<title>` and Chromium’s placeholder favicon
    if (null === metadata.title)
    metadata.title = documentTitle.trim();

    if (null === metadata.thumbnail) {
      var thumbnail = "https://www.google.com/s2/favicons?domain=" + link.hostname + "&sz=256";

      // ...
      response = await requestUrl({contentType: /* --> image/* */ DEFAULT_RESPONSE_HEADERS["Content-Type"], headers: DEFAULT_RESPONSE_HEADERS, method: "GET", "throw": false, url: thumbnail});

      if (response.status >= 200 && response.status < 300) metadata.thumbnail = thumbnail;
      else console.info("Unable to preview link thumbnail; Status " + response.status)
    }

    // ... ->> Format `metadata`
    metadata.description = (metadata.description || "").length > 80 ? sanitize(metadata.description.slice(0, 79)) + "&hellip;" : null !== metadata.description ? sanitize(metadata.description) : null; // ->> More than 80 characters
    metadata.title       = sanitize(metadata.title).replace(/[\n\r]/g, "");

    // ...
    cursorPosition = editor.getCursor();
    preview        = (
      "<div class=link-preview id=" + Array.from(crypto.getRandomValues(new Uint8Array(16))).map(byte => byte.toString(16).padStart(2, '0')).join("") + ">"                                                           +
        "<button class=link-preview-close><span>&#x1F6AB;&#xFE0F;</span></button>"                                                                                                                                    +
        "<div class=link-preview-thumbnail" + (null !== metadata.thumbnail ? " style=\"background-image: url('" + metadata.thumbnail + "') !important\"" : "") + "><a href=\"" + link + "\" target=_blank></a></div>" +
        "<div class=link-preview-content>"                                                                                                                                                                            +
          "<h1 class=link-preview-title><a href=\"" + link.href + "\" target=_blank>" + metadata.title                                              + "</a></h1>"                                                     +
          "<p class=link-preview-description>"                                        + (null !== metadata.description ? metadata.description : "") + "</p>"                                                          +
          "<a class=link-preview-url href=\"" + link.href + "\" target=_blank>"       + link.href                                                   + "</a>"                                                          +
        "</div>"                                                                                                                                                                                                      +
        "<div class=link-preview-end><!-- Do not edit --></div>"                                                                                                                                                      +
      "</div>"
    );

    cursorPosition.ch === editorSelectionCursorPosition.ch && cursorPosition.line === editorSelectionCursorPosition.line ? editor.replaceSelection(preview) : editor.replaceRange(preview, cursorPosition)
  }
}

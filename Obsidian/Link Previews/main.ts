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
    destroy() {
      this.view.dom.removeEventListener("mousedown", this.handler)
    }

    onMouseDown(event) {
      if (LinkPreviewPlugin.removePreview(LinkPreviewPlugin.getAncestorElementByClassName(LinkPreviewPlugin.getAncestorElementByClassName(event.target, "link-preview-close"), "link-preview"), this.view)) {
        event.preventDefault ();
        event.stopPropagation()
      }
    }

    update(viewUpdate) {}
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
    var cursorPosition                = editor.getCursor();
    var editorSelection               = editor.somethingSelected() ? editor.getSelection().trim() : "";
    var editorSelectionCursorPosition = cursorPosition;
    var link                          = null;
    var response                      = null;
    var responseHeaders               = {"Access-Control-Allow-Headers": "Content-Type", "Access-Control-Allow-Origin": location.href, "Cache-Control": "no-cache", "Content-Type": "text/plain"};

    // ...
    if (false === LinkPreviewPlugin.isLink(editorSelection)) {
      new Notice  ("Select a link to turn into a preview");
      console.info((editor.somethingSelected() ? "Expected" : "Pending") + " link to preview");

      return
    }

    link     = editorSelection;
    response = await requestUrl({contentType: "text/plain", headers: responseHeaders, method: "GET", "throw": /* ->> Status code 400+ */ false, url: link}); // ->> `fetch(…)` without CORS restrictions

    if (response.status < 200 && response.status >= 300) {
      new Notice   ("Unable to preview link; Status " + response.status);
      console.error("Unable to preview link; Status " + response.status);

      return
    }

    else {
      var document            = new DOMParser().parseFromString(response.text, "text/html");
      var documentCollections = [document.links, document.getElementsByTagName("meta")];
      var documentTitle       = document.title;
      var metadata            = {description: null, thumbnail: null, title: null};
      var preview             = null;
      var ranks               = {description: 1, thumbnail: 1.0, title: 1};

      /* ... */
      function sanitize(text) {
        return text.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;").replace(/"/g, "&quot;").replace(/'/g, "&#039;")
      }

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

      /* ... ->> Parse `metadata` from link `<link>` and `<meta>` */
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

      // ... ->> Fallback `metadata` from link `<title>` and Chromium’s placeholder favicon
      if (null === metadata.title)
      metadata.title = documentTitle.trim();

      if (null === metadata.thumbnail) {
        var thumbnail = "https://www.google.com/s2/favicons?domain=" + new URL(link).hostname + "&sz=256";

        // ...
        response = await requestUrl({contentType: "text/plain", headers: responseHeaders, method: "GET", "throw": false, url: thumbnail});

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
            "<h1 class=link-preview-title><a href=\"" + link + "\" target=_blank>" + metadata.title                                              + "</a></h1>"                                                          +
            "<p class=link-preview-description>"                                   + (null !== metadata.description ? metadata.description : "") + "</p>"                                                               +
            "<a class=link-preview-url href=\"" + link + "\" target=_blank>"       + link                                                        + "</a>"                                                               +
          "</div>"                                                                                                                                                                                                      +
          "<div class=link-preview-end><!-- Do not edit --></div>"                                                                                                                                                      +
        "</div>"
      );

      if (cursorPosition.ch === editorSelectionCursorPosition.ch && cursorPosition.line === editorSelectionCursorPosition.line) {
        editor.replaceSelection(preview);
        return
      }

      editor.replaceRange(preview, cursorPosition)
    }
  }
}

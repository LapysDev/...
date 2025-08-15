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
      this.handler = this.onClick.bind(this);
      this.view.dom.addEventListener("click", this.handler)
    }

    /* ... */
    destroy()           { this.view.dom.removeEventListener("click", this.handler) }
    update (viewUpdate) {}

    onClick(event) {
      var preview           = LinkPreviewPlugin.getAncestorElementByClassName(event.target, "link-preview");
      var previewForRemoval = LinkPreviewPlugin.getAncestorElementByClassName(LinkPreviewPlugin.getAncestorElementByClassName(event.target, "link-preview-close"), "link-preview");

      // ...
      if (LinkPreviewPlugin.removePreview(previewForRemoval, this.view)) {
        event.preventDefault ();
        event.stopPropagation()
      } else LinkPreviewPlugin.copyPreviewURL(preview)
    }
  });

  /* ... */
  settings; // --> LinkPreviewPlugin

  /* ... */
  static async copyPreviewURL(preview) {
    if (null === preview) return false;
    var previewTitle = LinkPreviewPlugin.formatText((LinkPreviewPlugin.getPreviewTitle(preview) || {innerText: null}).innerText);
    var previewURL   = (LinkPreviewPlugin.getPreviewURL(preview) || {href: null}).href;
    var renavigator  = null;

    // ...
    if (null === previewURL)
    return false;

    try { renavigator = app.dom.appContainerEl.win.navigator }
    catch (error) { renavigator = typeof navigator !== "undefined" ? navigator : null }

    // ...
    if (null !== renavigator && "clipboard" in renavigator && typeof renavigator.clipboard.writeText === "function")
      await renavigator.clipboard.writeText(previewURL);

    else {
      var textarea = document.createElement("textarea");

      // ...
      textarea.style.position = "fixed";
      textarea.value          = previewURL;

      document.body.appendChild(textarea);
      textarea.focus           ();
      textarea.select          ();

      try { document.execCommand("copy") }
      catch (error) { return false }

      document.body.removeChild(textarea)
    }

    new Notice  ("Copied preview" + previewTitle.replace(/.+/, " $&"));
    console.info("Copied preview" + previewTitle.replace(/.+/, " $&"));

    return true
  }

  static formatText(text) { return null !== text ? '“' + text.replace(/\s/g, ' ').replace(/[“”]/g, '"') + '”' : "" }

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
  static getPreviewURL  (preview) { return preview.querySelector         ("a.link-preview-url") }
  static isLink         (text)    { return /^(http:\/\/www\.|https:\/\/www\.|http:\/\/|https:\/\/)?[0-z@]+([\-.]{1}[0-z@]+)*\.[a-z]{2,5}(:[0-9]{1,5})?(\/.*)?$/.test(text) }

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
      var previews = element.getElementsByClassName("link-preview");

      // ...
      for (var index = previews.length; index; )
      previews.item(--index).addEventListener("click", async function preview(event) {
        var pluginLeaf        = plugin.app.workspace.activeLeaf;
        var preview           = this;
        var previewForRemoval = LinkPreviewPlugin.getAncestorElementByClassName(LinkPreviewPlugin.getAncestorElementByClassName(event.target, "link-preview-close"), "link-preview");

        // ...
        if (null !== pluginLeaf && null !== previewForRemoval) {
          var pluginView      = plugin.app.workspace.getActiveViewOfType(MarkdownView)?.editor.cm;
          var pluginViewState = pluginLeaf.getViewState();

          // ...
          pluginViewState.state.mode   = "source";
          pluginViewState.state.source = false;
          await pluginLeaf.setViewState(pluginViewState);

          if (LinkPreviewPlugin.removePreview(plugin.app.workspace.getActiveViewOfType(MarkdownView)?.editor.cm.dom.querySelector(".link-preview[id=\"" + previewForRemoval.id + "\"]"), pluginView)) {
            event.preventDefault ();
            event.stopPropagation()
          } else LinkPreviewPlugin.copyPreviewURL(preview);

          pluginViewState.state.mode   = "preview";
          pluginViewState.state.source = false;
          await pluginLeaf.setViewState(pluginViewState)
        } else LinkPreviewPlugin.copyPreviewURL(preview)
      })
    });
  }

  onunload() {}

  static removePreview(preview, view) {
    if (null !== preview) {
      var previewBeginPosition = view.posAtDOM(preview, 0);
      var previewEndPosition   = view.posAtDOM(LinkPreviewPlugin.getLastChildElementByClassName(preview, "link-preview-end") || preview, 0);
      var previewTitle         = LinkPreviewPlugin.formatText((LinkPreviewPlugin.getPreviewTitle(preview) || {innerText: null}).innerText);

      // ...
      if (null !== previewBeginPosition && null !== previewEndPosition) {
        previewBeginPosition = view.state.doc.lineAt(previewBeginPosition).from;
        previewEndPosition   = view.state.doc.lineAt(previewEndPosition)  .to;

        new Notice   ("Removed preview" + previewTitle.replace(/.+/, " $&"));
        console.info ("Removed preview" + previewTitle.replace(/.+/, " $& ") + "at " + previewBeginPosition + "–" + previewEndPosition);
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
    function getMetadataSize(sizes) {
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

    function sanitizeLinkForCode             (link) { return link.replace(/\n/g, "%0A").replace(/```/g, "%60%60%60") }
    function sanitizeLinkForMarkdown         (link) { return link.replace(/\n/g, "%0A").replace(/</g, "%3C").replace(/>/g, "%3E") }
    function sanitizeLinkForMarkdownAttribute(link) { return link.replace(/\n/g, "%0A").replace(/"/g, "%22") }
    function sanitizeTextForMarkdown         (text) { return text.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;").replace(/"/g, "&quot;").replace(/'/g, "&#039;") }

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
    new Notice  ("Previewing link `" + link.href.replace(/`/g, "%60") + "`…");
    console.info("Previewing link `" + link.href.replace(/`/g, "%60") + "`…");

    // if (null !== metadataLink.hostname.match(/((i|preview)\.redd\.it|(|old\.|www\.)reddit\.com)/)) {
    //   var redditPostID = (metadataLink.pathname.match(/\/comments\/([a-z0-9]+)(?:\/|$)/) || [, null])[1];

    //   // ...
    //   if (null !== redditPostID) {
    //     var redditClientID     = "";
    //     var redditClientSecret = "";
    //     var redditPassword     = "";
    //     var redditUserAgent    = "";
    //     var redditUsername     = "";
    //     var redditResponse     = await requestUrl({body: /* --> URLSearchParams */ "grant_type=password&username=" + encodeURIComponent(redditUsername) + "&password=" + encodeURIComponent(redditPassword), headers: {"Authorization": "Basic " + btoa(redditClientID + ':' + redditClientSecret), "Content-Type": "application/x-www-form-urlencoded", "User-Agent": redditUserAgent}, method: "POST", "throw": false, url: "https://www.reddit.com/api/v1/access_token"});

    //     // ...
    //     if (null !== redditPostID) {
    //       special.data     = redditPostID;
    //       special.platform = "Reddit";

    //       if (redditResponse.status === 200 && typeof redditResponse.json.access_token === "string") /* ->> Avoid `.json == {error: "invalid_grant"}` */ {
    //         metadataLink                                    = new URL(null !== redditPostID ? "https://oauth.reddit.com/comments/" + redditPostID + "/.json?raw_json=1" : "https://www.reddit.com/domain/i.redd.it.json?url=" + encodeURIComponent(metadataLink.href) + "&raw_json=1");
    //         responseHeaders["Access-Control-Allow-Headers"] = (responseHeaders["Access-Control-Allow-Headers"] || []).split(/\s*,\s*/).concat(["Authorization", "User-Agent"]).join(", ");
    //         responseHeaders["Authorization"]                = (redditResponse.json.token_type || "bearer ") + ' ' + redditResponse.json.access_token;
    //         responseHeaders["User-Agent"]                   = redditUserAgent;
    //         special.method                                  = "reddit-oauth"
    //       }

    //       else {
    //         metadataLink   = new URL(metadataLink.href + ".json");
    //         special.method = "reddit-json"
    //       }
    //     }
    //   }
    // }

    // ... ->> Get `metadata` about the `link`
    for (var requesting = true; requesting; ) {
      requesting = false;
      response   = await requestUrl({contentType: "text/plain", headers: responseHeaders["Content-Type"], method: "GET", "throw": /* ->> Status code 400+ */ false, url: metadataLink.href}); // ->> `fetch(…)` without CORS restrictions

      // ... ->> Failed to fetch `link`
      if (null === response || response.status < 200 || response.status >= 300 || (function(special) {
        switch (special.method) {
          case "reddit-json":
          case "reddit-oauth": try { void response.json[0].data.children[0].data; return false } catch (error) {} return true; // ->> Assert JSON layout
          default:
        }

        return false
      })(special)) {
        console.error("Unable to " + (null !== special.method ? "(specially) " : "") + "preview link `" + metadataLink.href.replace(/`/g, "%60") + "`; Status " + response.status, response);

        // ...
        if (link === metadataLink)
          new Notice("Unable to " + (null !== special.method ? "(specially) " : "") + "preview link `" + metadataLink.href.replace(/`/g, "%60") + "`; Status " + response.status);

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
        metadata.thumbnail   = null !== metadataLink.pathname.match(/\/media\//) ? post.url_overridden_by_dest || null : null;
        metadata.title       = post.title;

        if (null !== metadata.thumbnail) {
          try { metadata.thumbnail = "preview" in post && 0 !== (post.preview.images || []).length ? post.preview.images[0].source.url : new URL(post.thumbnail).href }
          catch (error) { metadata.thumbnail = special.method === "reddit-oauth" ? "https://share.redd.it/preview/post/" + special.data : null } // ->> still requires Open Authorization
        }

        console.info("Previewing “special” " + special.platform + " link")
      } break;

      default: {
        var responseContentType   = (response.headers["content-type"] || response.headers["Content-Type"] || "text/html").split(';')[0];
        var responseText          = response.text;
        var responseTextFormatted = responseText.length >= 80 ? responseText.slice(0, 79) + '…' : responseText;

        // ...
        if (responseContentType.startsWith("image/")) {
          var extension = responseContentType.slice("image/".length);

          // ...
          switch (extension) {
            case "aces": case "apng": case "avci": case "avcs": case "avif":
            case "bmp":
            case "cgm":
            case "dicom-rle": case "dpx":
            case "emf":
            case "fits":
            case "g3fax": case "gif":
            case "heic": case "heic-sequence": case "heif": case "heif-sequence": case "hej2k": case "hsj2":
            case "ief": case "j2c": case "jaii": case "jais": case "jls": case "jp2": case "jpeg": case "jph": case "jphc": case "jpm": case "jpx": case "jxl": case "jxr": case "jxrA": case "jxrS": case "jxs": case "jxsc": case "jxsi": case "jxss":
            case "ktx": case "ktx2":
            case "naplps":
            case "png": case "prs.btif": case "prs.pti": case "pwg-raster":
            case "svg+xml":
            case "t38": case "tiff": case "tiff-fx":
            case "vnd.adobe.photoshop": case "vnd.airzip.accelerator.azv": case "vnd.blockfact.facti": case "vnd.clip": case "vnd.cns.inf2": case "vnd.dece.graphic": case "vnd.djvu": case "vnd.dwg": case "vnd.dxf": case "vnd.dvb.subtitle": case "vnd.fastbidsheet": case "vnd.fpx": case "vnd.fst": case "vnd.fujixerox.edmics-mmr": case "vnd.fujixerox.edmics-rlc": case "vnd.globalgraphics.pgb": case "vnd.microsoft.icon": case "vnd.mix": case "vnd.ms-modi": case "vnd.mozilla.apng": case "vnd.net-fpx": case "vnd.pco.b16": case "vnd.radiance": case "vnd.sealed.png": case "vnd.sealedmedia.softseal.gif": case "vnd.sealedmedia.softseal.jpg": case "vnd.svf": case "vnd.tencent.tap": case "vnd.valve.source.texture": case "vnd.wap.wbmp": case "vnd.xiff": case "vnd.zbrush.pcx":
            case "webp": case "wmf":
            case "x-emf": case "x-wmf": break;
            default:
              extension = metadataLink.href.split(/[\\\/]/);
              extension = extension[extension.length - 1];
              extension = extension.indexOf('.') !== -1 ? extension.replace(/[^\.]*\./, "") : extension
          }

          try {
            var blob   = new Blob([responseText], {type: "image/png"});
            var reader = new FileReader();

            // ...
            responseTextFormatted = await new Promise(function(resolve, reject) {
              reader.onerror = function(event) { reject (null) };
              reader.onload  = function(event) { resolve(event.target.result) };

              reader.readAsDataURL(blob)
            });

            responseTextFormatted = null !== responseTextFormatted ? responseTextFormatted : URL.createObjectURL(blob);
            responseTextFormatted = null !== responseTextFormatted ? responseTextFormatted : "data:image/" + extension + ";base64," + Buffer.from(responseText, "binary").toString("base64");
            responseTextFormatted = null !== responseTextFormatted ? responseTextFormatted : "data:image/" + extension + ";base64," + /* --> atob(…) */ responseText;
            responseTextFormatted = null; // TODO (Lapys) -> Figure this out somehow
            responseTextFormatted = null !== responseTextFormatted ? responseTextFormatted : link.href;
            preview               = "![" + new DOMParser().parseFromString(responseText, "text/html").title + "](" + responseTextFormatted + ')';

            console.info("Previewing static " + extension.toUpperCase() + " image `" + responseTextFormatted + '`')
          } catch (error) { console.error(error) } // ->> Assume MIME media type is supported
        }

        else if (responseContentType === "model/3mf")                                                                     preview = "```3mf\n" + "<!-- Previewed: " + sanitizeLinkForCode(link.href).replace(/\-\->/g, "--%3E") + " -->\n"        + responseTextFormatted + "\n```"; // ->> XML
        else if (responseContentType === "application/A2L")                                                               preview = "```a2l\n" + "// Previewed: "   + sanitizeLinkForCode(link.href) + '\n'                                       + responseTextFormatted + "\n```"; // ->> A2L
        else if (responseContentType === "application/aml")                                                               preview = "```aml\n"                                                                                                    + responseTextFormatted + "\n```"; // ->> XML
        else if (responseContentType === "application/ATFX")                                                              preview = "```atfx\n"                                                                                                   + responseTextFormatted + "\n```"; // ->> XML
        else if (responseContentType === "application/ATXML")                                                             preview = "```atxml\n"                                                                                                  + responseTextFormatted + "\n```"; // ->> XML
        else if (responseContentType === "application/vnd.gentoo.ebuild")                                                 preview = "```bash\n" + "# Previewed: "  + sanitizeLinkForCode(link.href)                        + '\n'                 + responseTextFormatted + "\n```"; // ->> Bash
        else if (null !== responseContentType.match(/[\/\-\+]cbor($|\-)/))                                                preview = "```cbor\n" + "# Previewed: "  + sanitizeLinkForCode(link.href)                        + '\n'                 + responseTextFormatted + "\n```"; // ->> CBOR
        else if (responseContentType === "text/css")                                                                      preview = "```css\n"  + "/* Previewed: " + sanitizeLinkForCode(link.href).replace(/\*\//g, "*/") + " */\n"              + responseTextFormatted + "\n```"; // ->> CSS
        else if (null !== responseContentType.match(/^text\/csv(\-schema)$/))                                             preview = "```csv\n"                                                                                                    + responseTextFormatted + "\n```"; // ->> CSV
        else if (responseContentType === "application/vnd.gentoo.eclass")                                                 preview = "```eclass\n"     + "# Previewed: "  + sanitizeLinkForCode(link.href) + '\n'                                  + responseTextFormatted + "\n```"; // ->> EClass
        else if (responseContentType === "application/vnd.hp-HPGL")                                                       preview = "```hgpl\n"       + "CO “"           + sanitizeLinkForCode(link.href) + "”;"                                  + responseTextFormatted + "\n```"; // ->> HPGL
        else if (null !== responseContentType.match(/^(|application|text)\/((ecma|java)script|node|vnd\.cab\-jscript)$/)) preview = "```javascript\n" + "// Previewed: " + sanitizeLinkForCode(link.href) + '\n'                                  + responseTextFormatted + "\n```"; // ->> JavaScript (Node.js, …)
        else if (responseContentType === "application/cwl")                                                               preview = "```json\n"                                                                                                   + responseTextFormatted + "\n```"; // ->> JSON
        else if (null !== responseContentType.match(/(^application\/|\+)json(|\-seq)$/))                                  preview = "```json\n"                                                                                                   + responseTextFormatted + "\n```"; // ->> JSON
        else if (responseContentType === "text/markdown")                                                                 preview = "```markdown\n" + "<!-- Previewed: " + sanitizeLinkForCode(link.href).replace(/\-\->/g, "--%3E") + " -->\n"   + responseTextFormatted + "\n```"; // ->> Markdown
        else if (responseContentType === "text/mathematica")                                                              preview = "```mathematica\n"                                                                                            + responseTextFormatted + "\n```"; // ->> Mathematica
        else if (responseContentType === "text/mizar")                                                                    preview = "```mizar\n"                                                                                                  + responseTextFormatted + "\n```"; // ->> Mizar
        else if (responseContentType === "model/vnd.opengex")                                                             preview = "```opengex\n"  + "/* Previewed: "   + sanitizeLinkForCode(link.href).replace(/\*\//g, "*/")     + " */\n"    + responseTextFormatted + "\n```"; // ->> JSON
        else if (null !== responseContentType.match(/^(application|text)\/sgml$/))                                        preview = "```sgml\n"     + "<!-- Previewed: " + sanitizeLinkForCode(link.href).replace(/\-\->/g, "--%3E") + " -->\n"   + responseTextFormatted + "\n```"; // ->> Standard Graphics Markup Language
        else if (responseContentType === "text/turtle")                                                                   preview = "```turtle\n"                                                                                                 + responseTextFormatted + "\n```"; // ->> Turtle
        else if (responseContentType === "text/vtt")                                                                      preview = "```vtt\n" + "NOTE Previewed: " + sanitizeLinkForCode(link.href) + '\n'                                       + responseTextFormatted + "\n```"; // ->> WebVTT
        else if (responseContentType === "model/vnd.vtu")                                                                 preview = "```vtu\n"                                                                                                    + responseTextFormatted + "\n```"; // ->> XML
        else if (responseContentType === "text/wgsl")                                                                     preview = "```wgsl\n" + "// Previewed: " + sanitizeLinkForCode(link.href) + '\n'                                        + responseTextFormatted + "\n```"; // ->> WebGPU Shading Language
        else if (null !== responseContentType.match(/^application\/pkcs7\-[A-z\-]+$/))                                    preview = "```pkcs7\n"                                                                                                  + responseTextFormatted + "\n```"; // ->> PKCS #12
        else if (responseContentType === "application/pkcs8")                                                             preview = "```pkcs8\n"                                                                                                  + responseTextFormatted + "\n```"; // ->> PKCS #12
        else if (responseContentType === "application/pkcs10")                                                            preview = "```pkcs10\n"                                                                                                 + responseTextFormatted + "\n```"; // ->> PKCS #12
        else if (responseContentType === "application/pkcs12")                                                            preview = "```pkcs12\n"                                                                                                 + responseTextFormatted + "\n```"; // ->> PKCS #12
        else if (responseContentType === "application/sparql-query")                                                      preview = "```sparql\n"  + "# Previewed: "  + sanitizeLinkForCode(link.href) + '\n'                                     + responseTextFormatted + "\n```"; // ->> SPARQL
        else if (responseContentType === "application/sql")                                                               preview = "```sql\n"     + "-- Previewed: " + sanitizeLinkForCode(link.href) + '\n'                                     + responseTextFormatted + "\n```"; // ->> SQL
        else if (responseContentType === "application/texinfo")                                                           preview = "```texinfo\n" + "@c Previewed: " + sanitizeLinkForCode(link.href) + '\n'                                     + responseTextFormatted + "\n```"; // ->> Texinfo
        else if (responseContentType === "application/toml")                                                              preview = "```toml\n"    + "# Previewed: "  + sanitizeLinkForCode(link.href) + '\n'                                     + responseTextFormatted + "\n```"; // ->> TOML
        else if (null !== responseContentType.match(/^application\/pkix\-[A-z\-]+$/))                                     preview = "```x509\n"                                                                                                   + responseTextFormatted + "\n```"; // ->> X.509
        else if (null !== responseContentType.match(/[\/\-\+](XML|xml)($|\-|\+)/))                                        preview = "```xml\n"  + "<!-- Previewed: " + sanitizeLinkForCode(link.href).replace(/\-\->/g, "--%3E") + " -->\n"       + responseTextFormatted + "\n```"; // ->> XML
        else if (responseContentType === "application/prs.cyn")                                                           preview = "```yaml\n"                                                                                                   + responseTextFormatted + "\n```"; // ->> YAML
        else if (null !== responseContentType.match(/[\/\-\+]yaml($|\-|\+)/))                                             preview = "```yaml\n" + "# Previewed: "  + sanitizeLinkForCode(link.href) + '\n'                                        + responseTextFormatted + "\n```"; // ->> YAML
        else if (responseContentType === "application/yang")                                                              preview = "```yang\n" + "// Previewed: " + sanitizeLinkForCode(link.href) + '\n'                                        + responseTextFormatted + "\n```"; // ->> YANG

        else if (null !== responseContentType.match(/^application\/(cdmi\-[A-z]+|cose(|\-key(|\-set))|dashdelta|did|(dpop|eat|entity\-statement|jwk\-set|kb|logout|oauth\-authz\-req|provided\-claims|resolve\-response|secevent|token\-introspection|trust\-mark(|\-delegation)|vc|vnd\.ga4gh\.passport|vp)\+jwt|jose|jsonpath|jwt|linkset|multipart\-core|private\-token\-[A-z\-]+|prs\.implied\-structure|scvp\-(cv|vp)\-(request|response)|sd\-jwt|vc(|\+cose)|vnd\.(artsquare|astraea\-software\.iota|audiograph|banana\-accounting|bbf\.usp\.(error|msg)|balsamiq\.bmpr|bint\.med\-content|blink\-idb\-value\-wrapper|blueice\.multipass|bmi|bpf(|3)|wordlift|vc(|\+cose)))$/))                                                                                  preview = "```json\n" + responseTextFormatted + "\n```"; // ->> JSON
        else if (null !== responseContentType.match(/^application\/(index\.[A-z]+|IOTP|ipfix|ipp|marc|mp21|mpeg4\-iod\-xmt|PDX|prs\.(alvestrand\.titrax\-sheet|cww)|srgs|vnd\.(adobe\.formscentral\.fcdt|businessobjects|canon\-[a-z]+|cel|ims\.imsccv1p[1-3]|intu\.(qbo|qfx)|oma\.push|omaloc\-supl\-init|oma\-scws\-config|onepager(|tam[px]|tat(|p|x))|onvif\.metadata|openeye\.oeb|openxmlformats\-officedocument\.(presentationml\.(presentation|slide(|show)|template|spreadsheetml\.(sheet|template)|vmlDrawing|wordprocessingml\.(document|template)))|osa\.netdeploy|osgeo\.mapguide\.package|osgi\.subsystem|patientecommsdoc|piaccess\.application\-licence|pmi\.widget|pocketlearn|recordare\.musicxml|xfdl(|\.webform)|yamaha\.openscoreformat)|xfdf)$/)) preview = "```xml\n"  + responseTextFormatted + "\n```"; // ->> XML

        else if (
          responseContentType.startsWith("haptics/")                                       ||
          responseContentType.startsWith("message/")                                       ||
          responseContentType.startsWith("model/")                                         ||
          responseContentType.startsWith("multipart/")                                     ||
          (responseContentType.startsWith("text/") && responseContentType !== "text/html") ||
          null !== responseContentType.match(/^application\/(1d\-interleaved\-parityfec|activemessage|andrew\-inset|applefile|at\+jwt|ATF|link\-format|mbox|n\-triples|news\-[A-z]+|passport|pdf|pem\-certificate\-chain|pgp\-[a-z]+|pkixcmp|postscript|prs\.[a-z\-]+|QSIG|raptorfec|relax\-ng\-compact\-syntax|remote\-printing|riscos|rpki\-[a-z\-]+|rtf|rtploopback|rtx|sbe|sdp|sen(|s)ml\-exi|sgml\-open\-catalog|sieve|simple\-message\-summary|slate|trickle\-ice\-sdpfrag|trig|tzif|vnd\.(apple\.[a-z]+|aristanetworks\.swi|autopackage|bluetooth\.(ep|le)\.oob|cendio\.thinlinc\.clientconf|chess\-pgn|gentoo\.manifest|groove\-vcard|hsl|oma\-scws\-http\-(request|response)|wfa\.wsc|wolfram\.mathematica(\.package))|x\-www\-form\-urlencoded)$/)
        ) preview = "```\n" + responseTextFormatted + "\n```";

        else if (responseContentType !== "text/html") {
          var width = 2;

          // ...
          responseTextFormatted = "";

          for (var index = Math.min(responseText.length, 80); index; )
          width = Math.max(width, responseText.charCodeAt(--index).toString(16).length);

          for (var index = Math.min(responseText.length, 80); index; ) {
            var code = responseText.charCodeAt(--index).toString(16).toUpperCase();
            responseTextFormatted = '0'.repeat(width - code.length) + code + ' ' + responseTextFormatted
          }

          preview = "```\n" + (responseText.length >= 80 ? responseTextFormatted + '…' : responseTextFormatted.slice(0, -1)) + "\n```"
        }

        // ... ->> Can handle more than `text/html` MIME media type
        if (null === preview) {
          var document = new DOMParser().parseFromString(responseText, "text/html");

          // ... ->> Parse `metadata` from `document` `<link>` and `<meta>`
          documentTitle = document.title;
          documentCollections.push(document.links, document.getElementsByTagName("meta"));

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

              case "apple-touch-icon":          subranks.thumbnail = 9.0 + getMetadataSize(subvalue || []); break; // --> rel
              case "apple-touch-startup-image": subranks.thumbnail = 8.0;                                   break; // --> rel
              case "fluid-icon":                subranks.thumbnail = 6.0;                                   break; // --> rel
              case "icon":                      subranks.thumbnail = 4.0 + getMetadataSize(subvalue || []); break; // --> rel
              case "mask-icon":                 subranks.thumbnail = 7.0;                                   break; // --> rel
              case "og:image":                  subranks.thumbnail = 11.0;                                  break; // --> property
              case "shortcut icon":             subranks.thumbnail = 2.0 + getMetadataSize(subvalue || []); break; // --> rel
              case "twitter:image":             subranks.thumbnail = 12.0;                                  break; // --> name

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
    }

    if (null === preview) {
      // ... ->> Fallback `metadata` from link `<title>` and Chromium’s placeholder favicon
      if (null === metadata.title)        metadata.title = documentTitle.trim();
      if (0    === metadata.title.length) metadata.title = link.href    .trim();

      if (null === metadata.thumbnail) {
        var thumbnail = "https://www.google.com/s2/favicons?domain=" + link.hostname + "&sz=256";

        // ...
        response = await requestUrl({contentType: /* --> image/* */ DEFAULT_RESPONSE_HEADERS["Content-Type"], headers: DEFAULT_RESPONSE_HEADERS, method: "GET", "throw": false, url: thumbnail});

        if (response.status >= 200 && response.status < 300) metadata.thumbnail = thumbnail;
        else console.info("Unable to preview link thumbnail; Status " + response.status, response)
      }

      // ... ->> Format `metadata`
      metadata.description = (metadata.description || "").length >= 80 ? sanitizeTextForMarkdown(metadata.description.slice(0, 79)) + "&hellip;" : null !== metadata.description ? sanitizeTextForMarkdown(metadata.description) : null; // ->> More than 80 characters
      metadata.title       = sanitizeTextForMarkdown(metadata.title).replace(/[\n\r]/g, "");

      // ...
      preview = (
        "<div class=link-preview id=" + Array.from(crypto.getRandomValues(new Uint8Array(16))).map(byte => byte.toString(16).padStart(2, '0')).join("") + ">"                                                                                                                                    +
          "<button class=link-preview-close><span>&#x1F6AB;&#xFE0F;</span></button>"                                                                                                                                                                                                             +
          "<div class=link-preview-thumbnail" + (null !== metadata.thumbnail ? " style='background-image: url(\"" + sanitizeLinkForMarkdownAttribute(metadata.thumbnail) + "\") !important'" : "") + "><a href=\"" + sanitizeLinkForMarkdownAttribute(link.href) + "\" target=_blank></a></div>" +
          "<div class=link-preview-content>"                                                                                                                                                                                                                                                     +
            "<h1 class=link-preview-title><a href=\"" + sanitizeLinkForMarkdownAttribute(link.href) + "\" target=_blank>" + metadata.title                                              + "</a></h1>"                                                                                            +
            "<p class=link-preview-description>"                                                                          + (null !== metadata.description ? metadata.description : "") + "</p>"                                                                                                 +
            "<a class=link-preview-url href=\"" + sanitizeLinkForMarkdownAttribute(link.href) + "\" target=_blank>"       + sanitizeLinkForMarkdown(link.href)                          + "</a>"                                                                                                 +
          "</div>"                                                                                                                                                                                                                                                                               +
          "<div class=link-preview-end><!-- Do not remove --></div>"                                                                                                                                                                                                                               +
        "</div>"
      )
    }

    cursorPosition = editor.getCursor();
    cursorPosition.ch === editorSelectionCursorPosition.ch && cursorPosition.line === editorSelectionCursorPosition.line ? editor.replaceSelection(preview) : editor.replaceRange(preview, cursorPosition)
  }
}

function getCSSPropertyValue(element, property, resolve, strict) {
  var layers      = [""];
  var styleSheets = []; // NOTE (Lapys) -> Iterated/ updated in reverse
  var rules       = []; // TODO (Lapys) -> Pending memoization

  /* ... */
  function compare(specificityA, specificityB) {
    return (
      specificityA[0] !== specificityB[0] ? specificityA[0] - specificityB[0] :
      specificityA[1] !== specificityB[1] ? specificityA[1] - specificityB[1] :
                                            specificityA[2] - specificityB[2]
    )
  }

  function matches(element, selector) {
    try {
      if      (typeof element.matches               === "function") return element.matches              (selector);
      else if (typeof element.mozMatchesSelector    === "function") return element.mozMatchesSelector   (selector);
      else if (typeof element.msMatchesSelector     === "function") return element.msMatchesSelector    (selector);
      else if (typeof element.oMatchesSelector      === "function") return element.oMatchesSelector     (selector);
      else if (typeof element.webkitMatchesSelector === "function") return element.webkitMatchesSelector(selector)
    } catch (error) { /* --> DOMException | SyntaxError */ }

    return false
  }

  function max(selectors) {
    var selectorList      = [];
    var selectorListIndex = 0;
    var specificity       = {0: 0, 1: 0, 2: 0};

    // ...
    for (var depth = 0, index = 0; index !== selectors.length; ++index)
    switch (selectors[index]) {
      case '(': ++depth; break;
      case ')': --depth; break;
      case ',': if (0 === depth) {
        void selectorList.push(selectors.slice(selectorListIndex, index)/* --> .trim() */.replace(/(^|)\s+($|)/g, ""));
        selectorListIndex = index + 1
      }
    }

    void selectorList.push(selectors.slice(selectorListIndex)/* --> .trim() */.replace(/(^|)\s+($|)/g, ""));

    for (var index = 0; index !== selectorList.length; ++index) {
      var selectorSpecificity = null;

      // ...
      try { selectorSpecificity = rank(selectorList[index]) }
      catch (error) { /* --> InternalError | RangeError */ }

      specificity = null !== selectorSpecificity && compare(selectorSpecificity, specificity) > 0 ? selectorSpecificity : specificity
    }

    return specificity
  }

  function rank(selector) /* TODO (Lapys) -> Same scope as `compare(…)` and `matches(…)` */ {
    var specificity = {0: 0, 1: 0, 2: 0};

    // ...
    for (var index = 0; index !== selector.length; )
    switch (selector[index]) {
      case '#': for (++specificity[0], ++index; index !== selector.length; ++index) { if (!/[\w-]/.test(selector[index])) break } break; // ->> ID        selectors
      case '.': for (++specificity[1], ++index; index !== selector.length; ++index) { if (!/[\w-]/.test(selector[index])) break } break; // ->> Class     selectors
      case '[': for (++specificity[1];          index !== selector.length; )        { if (selector[index++] === ']')      break } break; // ->> Attribute selectors
      case ':': ++index; {
        var isPseudoElement = selector[index] === ':'; if    (isPseudoElement)                                            ++index;
        var pseudoIndex     = index;                   while (index !== selector.length && /[\w-]/.test(selector[index])) ++index;
        var pseudo          = selector.slice(pseudoIndex, index).toLowerCase();

        // ...
        if (selector[index] !== '(') ++specificity[isPseudoElement || pseudo === "after" || pseudo === "backdrop" || pseudo === "before" || pseudo === "file-selector-button" || pseudo === "first-letter" || pseudo === "first-line" || pseudo === "grammar-error" || pseudo === "marker" || pseudo === "placeholder" || pseudo === "selection" || pseudo === "spelling-error" ? 2 : 1];
        else {
          var selectors = null;

          // ...
          for (var depth = 0, subindex = index; subindex !== selector.length; ++subindex) {
            if      (selector[subindex] === '(') { ++depth }
            else if (selector[subindex] === ')') { --depth; if (0 === depth) { selectors = selector.slice(index + 1, subindex); index = subindex + 1; break } }
          }

          if (null === selectors) { selectors = selector.slice(index + 1); index = selector.length }

          switch (pseudo) {
            case "has":       case "is": case "not": { var selectorsSpecificity = max(selectors); specificity[0] += selectorsSpecificity[0]; specificity[1] += selectorsSpecificity[1]; specificity[2] += selectorsSpecificity[2] } break;
            case "nth-child": case "nth-last-child": { var subselectorIndex = selectors.search(/\bof\b/); if (subselectorIndex !== -1) { var subselectorSpecificity = max(selectors.slice(subselectorIndex + /* --> "of" */ 2)/* --> .trim() */.replace(/(^|)\s+($|)/g, "")); specificity[0] += subselectorSpecificity[0]; specificity[1] += subselectorSpecificity[1]; specificity[2] += subselectorSpecificity[2] } ++specificity[1] } break;
            case "where":                            continue; break; // ->> Explicitly `0` specificity
            default:                                 ++specificity[1] // ->> Pseudo-class selectors
          }
        }
      } break;
      default:
        if (!/[A-z_\\]/.test(selector[index])) ++index;                                                               // ->> Combinators, whitespace, or universal selector `*`
        else for (++specificity[2]; index !== selector.length; ++index) { if (!/[\w-]/.test(selector[index])) break } // ->> Type selectors
    }

    return specificity
  }

  // ...
  for (var index = document.styleSheets.length; index; ) {
    var styleSheet = document.styleSheets.item(--index);

    try { void styleSheets.push({layer: layers[0], rules: styleSheet.cssRules}) } catch (error) { // --> SecurityError
    try { void styleSheets.push({layer: layers[0], rules: styleSheet.rules}) }    catch (error) { /* --> SecurityError */ if (strict) return null } }
  }

  for (var order = 0; styleSheets.length; ) {
    var styleSheet = styleSheets.pop(); // --> styleSheets[--styleSheets.length]
    var layer      = styleSheet.layer;

    for (var index = 0, length = styleSheet.rules.length; index !== length; ++index) {
      var rule = styleSheet.rules[index];

      // ...
      if (typeof CSSContainerRule === "function" && rule instanceof CSSContainerRule && typeof rule.cssRules === "object" && typeof rule.containerName === "string" && typeof rule.containerQuery === "string") {
        // TODO (Lapys) -> Relatively straight-forward for `aspect-ratio`, `block-size`, `height`, `inline-size`, `orientation`, and `width` queries, but `scroll-state(…)` and `style(…)` are non-trivial
        void rule.containerName;
        void rule.containerQuery
      }

      else if (typeof CSSLayerBlockRule === "function" && rule instanceof CSSLayerBlockRule && typeof rule.cssRules === "object" && typeof rule.name === "string") /* --> @layer … {} */ {
        var sublayer = rule.name !== "" ? (layer !== "" ? layer + '.' : "") + rule.name : layer;

        if (layers.indexOf(sublayer) === -1) { void layers.splice(layers.indexOf(layer), 0, sublayer) }
        void styleSheets.push({layer: sublayer, rules: rule.cssRules})
      }

      else if (typeof CSSLayerStatementRule === "function" && rule instanceof CSSLayerStatementRule && typeof rule.nameList === "object") /* --> @layer … */ {
        for (var subindex = 0; subindex !== rule.nameList.length; ++subindex) {
          var sublayer = rule.nameList[subindex];

          if (layers.indexOf(sublayer) === -1)
          void layers.push(sublayer)
        }
      }

      else if (typeof CSSScopeRule === "function" && rule instanceof CSSScopeRule && typeof rule.cssRules === "object" && typeof rule.end === "string" && typeof rule.start === "string") {
        var scoped = false;

        // ...
        if (null === rule.start) scoped = true;
        else {
          var root = null;

          // ...
          for (var node = element; null !== node && node.nodeType === /* --> Node.ELEMENT_NODE */ 0x1; node = node.parentNode)
          if (matches(node, rule.start)) { root = node; break }

          // ...
          if (element === root) scoped = true;
          else if (null !== root && null !== rule.end) {
            scoped = true;

            for (var node = element.parentNode; null !== node && node.nodeType === /* --> Node.ELEMENT_NODE */ 0x1 && node !== root; node = node.parentNode)
            if (matches(node, rule.end)) { scoped = false; break }
          }
        }

        if (scoped)
        void styleSheets.push({layer: layer, rules: rule.cssRules})
      }

      else switch (typeof rule.type !== "number" ? /* --> CSSRule.UNKNOWN_RULE */ 0x00 : rule.type) {
        // --> CSSRule.STYLE_RULE === CSSStyleRule::type
        case 0x01: void rules.push({layer: layer, order: order++, rule: rule}); break;

        // --> CSSRule.IMPORT_RULE === CSSImportRule::type --> @import
        case 0x03: if (typeof rule.styleSheet === "object") {
          try { void styleSheets.push({layer: layer, rules: rule.styleSheet.cssRules}) } catch (error) { // --> SecurityError
          try { void styleSheets.push({layer: layer, rules: rule.styleSheet.rules}) }    catch (error) { /* --> SecurityError */ if (strict) return null } }
        } break;

        // --> CSSRule.MEDIA_RULE === CSSMediaRule::type --> @media
        case 0x04: if (typeof rule.cssRules === "object" && typeof matchMedia === "function") {
          if (matchMedia(typeof rule.media === "object" && typeof rule.media.mediaText === "string" ? rule.media.mediaText : typeof rule.conditionText === "string" ? rule.conditionText : "all"))
          void styleSheets.push({layer: layer, rules: rule.cssRules})
        } break;

        case 0x07: continue; // --> CSSRule.KEYFRAMES_RULE === CSSKeyframesRule::type --- CODE (Lapys) -> @keyframes
        case 0x08: continue; // --> CSSRule.KEYFRAME_RULE  === CSSKeyframeRule ::type --- TODO (Lapys) -> `Animation[] Element::getAnimations({subtree: false})` is available but computing mid-interpolations between CSS property values is non-trivial

        // --> CSSRule.SUPPORTS_RULE === CSSSupportsRule::type --> @supports
        case 0x0C: if (typeof rule.cssRules === "object" && typeof CSS === "object" && typeof CSS.supports === "function") try {
          if (CSS.supports(typeof rule.conditionText === "string" ? rule.conditionText : "display: block"))
          void styleSheets.push({layer: layer, rules: rule.cssRules})
        } catch (error) { if (strict) return null } break;

        case 0x05: // --> CSSRule.FONT_FACE_RULE           === CSSFontFaceRule         ::type                                                                                        --> @font-face
        case 0x06: // --> CSSRule.PAGE_RULE                === CSSPageRule             ::type                                                                                        --> @page
        case 0x09: // --> CSSRule.MARGIN_RULE              === CSSMarginRule           ::type                                                                                        --> …
        case 0x0A: // --> CSSRule.NAMESPACE_RULE           === CSSNamespaceRule        ::type                                                                                        --> @namespace
        case 0x0B: // --> CSSRule.COUNTER_STYLE_RULE       === CSSCounterStyleRule     ::type                                                                                        --> @counter-style
        case 0x0E: // --> CSSRule.FONT_FEATURE_VALUES_RULE === CSSFontFeatureValuesRule::type                                                                                        --> @font-feature-values
        default:   // --> CSSRule.UNKNOWN_RULE {0x00} | CSSRule.CHARSET_RULE {0x02} | CSSRule.DOCUMENT_RULE {0x0D} | CSSRule.REGION_STYLE_RULE {0x10} | CSSRule.VIEWPORT_RULE {0x0E} --> @container, @document, …
          if (typeof rule.cssRules === "object")
          void styleSheets.push({layer: layer, rules: rule.cssRules})
      }
    }
  }

  for (var cascaded = false, unlayered = []; !cascaded; ) {
    var information = null; // --> {matches: false, order: < 0, priority: "", specificity: {0: 0, 1: 0, 2: 0}, value: null}

    // ...
    cascaded = true;

    for (var index = 0, specificity = /* --> {0: 0, 1: 0, 2: 0} */ null; index !== rules.length; ++index) /* --> with (rules[index]) */ {
      var rule           = rules[index].rule;
      var selectors      = rule.selectorText.split(/\s*,\s*/);
      var style          = rule.style;
      var subinformation = {matches: false, layer: rules[index].layer, order: rules[index].order, priority: style.getPropertyPriority(property), specificity: null, value: style.getPropertyValue(property)};

      // ...
      for (var subindex = selectors.length; subindex--; )
      if (matches(element, selectors[subindex])) {
        var selectorSpecificity = rank(selectors[subindex]);

        specificity            = null === specificity || compare(selectorSpecificity, specificity) > 0 ? selectorSpecificity : specificity;
        subinformation.matches = true
      }

      subinformation.specificity = specificity;

      if (subinformation.matches && null !== subinformation.specificity && subinformation.value !== "" && unlayered.indexOf(subinformation.layer) === -1) {
        var precedes = false;

        // ...
        if (null === information || (information.priority !== "important" && subinformation.priority === "important"))
          precedes = true;

        else if (information.priority === subinformation.priority) {
          if (information.layer !== subinformation.layer)                                       precedes = (layers.indexOf(information.layer) - layers.indexOf(subinformation.layer)) * (subinformation.priority === "important" ? -1 : +1) < 0;
          else { var comparison = compare(information.specificity, subinformation.specificity); precedes = comparison < 0 || (0 === comparison && information.order < subinformation.order) }
        }

        // ...
        if (precedes)
        information = subinformation
      }
    }

    if (null !== information && resolve)
    for (var resolved = false; !resolved; )
    switch (information.value) {
      case "inherit": {
        cascaded = false;
        element  = element.parentNode;
        resolved = true;

        if (null === element || element.nodeType !== /* --> Node.ELEMENT_NODE */ 0x1)
        return null
      } break;

      case "initial": switch (property) {
        case "align-content":         case "align-items": case "animation-direction": case "animation-range-end": case "animation-range-start":
        case "background-blend-mode": case "box-direction":
        case "column-gap":            case "container-type": case "content":
        case "gap":                   case "initial-letter": case "justify-content": case "mix-blend-mode":
        case "place-content":         case "position-try-order":
        case "reading-flow":          case "row-gap":
        case "scroll-snap-stop":
        case "text-box": case "transition-behavior":
        case "unicode-bidi":
          return "normal";

        case "align-self": case "animation-timeline": case "aspect-ratio":
        case "block-size": case "bottom":             case "break-after": case "break-before": case "break-inside":
        case "clip":       case "column-count":       case "column-width":
        case "flex-basis":
        case "grid-area": case "grid-auto-columns": case "grid-auto-rows": case "grid-column": case "grid-column-end": case "grid-column-start": case "grid-row": case "grid-row-end": case "grid-row-start":
        case "height":
        case "inline-size":       case "inset": case "inset-block": case "inset-block-end": case "inset-block-start": case "inset-inline": case "inset-inline-end": case "inset-inline-start": case "isolation":
        case "justify-self":      case "left":
        case "mask-border-width": case "mask-size":
        case "offset-anchor":     case "offset-rotate":    case "overflow-anchor":   case "overscroll-behavior": case "overscroll-behavior-block": case "overscroll-behavior-inline": case "overscroll-behavior-x": case "overscroll-behavior-y":
        case "page":              case "page-break-after": case "page-break-before": case "page-break-inside":   case "place-self":
        case "right":
        case "scroll-behavior": case "scroll-padding-block-end": case "scroll-padding-block-start": case "scroll-padding-bottom": case "scroll-padding-inline-end": case "scroll-padding-inline-start": case "scroll-padding-left": case "scroll-padding-right": case "scroll-padding-top": case "scrollbar-gutter":
        case "table-layout":    case "text-box-edge":            case "text-decoration-thickness":  case "top":                   case "touch-action":
        case "user-select":
        case "view-timeline-inset":
        case "width":   case "will-change":
        case "z-index": case "-moz-user-input":
          return "auto";

        case "alignment-baseline":
        case "vertical-align":
          return "baseline";

        case "anchor-name":     case "animation":        case "animation-fill-mode":    case "animation-name":           case "appearance":
        case "backdrop-filter": case "background-image": case "border-block-end-style": case "border-block-start-style": case "border-block-style": case "border-bottom-style":          case "border-image":             case "border-image-source":           case "border-inline-end-style": case "border-inline-start-style": case "border-inline-style": case "border-left-style": case "border-right-style": case "border-style": case "border-top-style": case "box-shadow":
        case "clear":           case "clip-path":        case "column-rule-style":      case "column-span":              case "contain":            case "contain-intrinsic-block-size": case "contain-intrinsic-height": case "contain-intrinsic-inline-size": case "contain-intrinsic-size":  case "contain-intrinsic-width":   case "container-name":      case "counter-increment": case "counter-reset":      case "counter-set":
        case 'd':
        case "filter": case "float":
        case "grid":   case "grid-template": case "grid-template-areas": case "grid-template-columns": case "grid-template-rows":
        case "line-clamp":
        case "margin-trim":         case "mask":            case "mask-border-source": case "mask-image": case "max-block-size": case "max-height": case "max-inline-size": case "max-width":
        case "offset-path":         case "outline-style":   case "overlay":
        case "perspective":         case "position-anchor": case "position-area": case "position-try-fallbacks":
        case "resize":              case "rotate":
        case "scale":               case "scroll-marker-group":  case "scroll-snap-align":     case "scroll-snap-type":     case "scroll-timeline-name": case "shape-outside":
        case "text-box-trim":       case "text-combine-upright": case "text-decoration":       case "text-decoration-line": case "text-emphasis-style":  case "timeline-scope": case "transform": case "transition": case "translate":
        case "vector-effect":       case "view-timeline-name":   case "view-transition-class": case "view-transition-name":
        case "-webkit-box-reflect": case "-webkit-mask-box-image":
          return "none";

        case "animation-delay":  case "animation-duration":
        case "transition-delay": case "transition-duration":
          return "0s";

        case "animation-iteration-count":
        case "border-image-width": case "box-flex-group": case "box-ordinal-group":
        case "flex-shrink":        case "flood-opacity":
        case "opacity":            case "stop-opacity": case "zoom":
          return '1';

        case "animation-timing-function":
        case "transition-timing-function":
          return "ease";

        case "backface-visibility": case "content-visibility":
        case "overflow":            case "overflow-block": case "overflow-inline": case "overflow-x": case "overflow-y":
          return "visible";

        case "background-clip":
        case "mask-clip": case "mask-origin":
          return "border-box";

        case "background-position-x":   case "background-position-y":
        case "-webkit-mask-position-x": case "-webkit-mask-position-y":
          return "0%";

        case "background-repeat":     case "mask-repeat":
        case "-webkit-mask-repeat-x": case "-webkit-mask-repeat-y":
          return "repeat";

        case "border": case "border-block": case "border-block-end": case "border-block-start": case "border-bottom": case "border-inline": case "border-inline-end": case "border-inline-start": case "border-left": case "border-right": case "border-top":
        case "column-rule":
          return "medium none currentcolor";

        case "border-block-color": case "border-block-end-color": case "border-block-start-color": case "border-bottom-color": case "border-color": case "border-inline-color": case "border-inline-end-color": case "border-inline-start-color": case "border-left-color": case "border-right-color": case "border-top-color":
        case "column-rule-color":
        case "text-decoration-color": case "text-emphasis-color":
          return "currentcolor";

        case "border-block-end-width": case "border-block-start-width": case "border-block-width": case "border-bottom-width": case "border-inline-end-width": case "border-inline-start-width": case "border-inline-width": case "border-left-width": case "border-right-width": case "border-top-width": case "border-width":
        case "column-rule-width":      case "outline-width":
          return "medium";

        case "border-bottom-left-radius": case "border-bottom-right-radius": case "border-end-end-radius": case "border-end-start-radius": case "border-image-outset": case "border-start-end-radius": case "border-start-start-radius": case "border-top-left-radius": case "border-top-right-radius": case "box-flex":
        case "cx":                        case "cy":
        case "flex-grow":
        case "margin":          case "margin-block":            case "margin-block-end": case "margin-block-start": case "margin-bottom": case "margin-inline": case "margin-inline-end": case "margin-inline-start": case "margin-left": case "margin-right": case "margin-top": case "mask-border-outset": case "mask-border-slice": case "min-block-size": case "min-height": case "min-inline-size": case "min-width":
        case "offset-distance": case "order":                   case "outline-offset":
        case "padding":         case "padding-block":           case "padding-block-end":         case "padding-block-start": case "padding-bottom": case "padding-inline": case "padding-inline-end": case "padding-inline-start": case "padding-left": case "padding-right": case "padding-top":
        case 'r':               case "reading-order":           case "rx":                        case "ry":
        case "scroll-margin":   case "scroll-margin-block-end": case "scroll-margin-block-start": case "scroll-margin-bottom": case "scroll-margin-inline-end": case "scroll-margin-inline-start": case "scroll-margin-left": case "scroll-margin-right": case "scroll-margin-top": case "shape-image-threshold": case "shape-margin":
        case 'x':               case 'y':                       case "-moz-force-broken-image-icon":
          return '0';

        case "background-size":      case "columns":
        case "scroll-padding-block": case "scroll-padding-inline":
          return "auto auto";

        case "border-radius":        case "scroll-padding":       return "0 0 0 0";
        case "box-align":            case "mask-border-repeat":   return "stretch";
        case "box-sizing":           case "-moz-float-edge":      return "content-box";
        case "display":              case "-moz-orient":          return "inline";
        case "flex-direction":       case "grid-auto-flow":       return "row";
        case "flood-color":          case "stop-color":           return "black";
        case "object-position":      case "perspective-origin":   return "50% 50%";
        case "scroll-margin-block":  case "scroll-margin-inline": return "0 0";
        case "scroll-timeline-axis": case "view-timeline-axis":   return "block";

        case "all":                         return "initial";
        case "animation-composition":       return "replace";
        case "animation-play-state":        return "running";
        case "animation-range":             return "normal normal";
        case "background":                  return "none 0% 0% / auto auto repeat scroll border-box padding-box transparent";
        case "background-attachment":       return "scroll";
        case "background-color":            return "transparent";
        case "background-origin":           return "padding-box";
        case "background-position":         return "0% 0%";
        case "border-image-repeat":         return "stretch stretch";
        case "border-image-slice":          return "100%";
        case "box-decoration-break":        return "slice";
        case "box-lines":                   return "single";
        case "box-orient":                  return "inline-axis";
        case "box-pack":                    return "start";
        case "color-interpolation":         return "sRGB";
        case "color-interpolation-filters": return "linearRGB";
        case "column-fill":                 return "balance";
        case "container":                   return "none/normal";
        case "field-sizing":                return "fixed";
        case "flex":                        return "0 1 auto";
        case "flex-flow":                   return "row nowrap";
        case "flex-wrap":                   return "nowrap";
        case "justify-items":               return "legacy";
        case "lighting-color":              return "white";
        case "mask-border":                 return "none 0/auto/0 stretch alpha";
        case "mask-border-mode":            return "alpha";
        case "mask-composite":              return "end";
        case "mask-mode":                   return "match-source";
        case "mask-position":               return "center";
        case "mask-type":                   return "luminance";
        case "object-fit":                  return "fill";
        case "offset":                      return "normal none 0/auto";
        case "outline":                     return "medium none invert,currentcolor";
        case "outline-color":               return "invert";
        case "overflow-clip-margin":        return "0px";
        case "place-items":                 return "normal legacy";
        case "position":                    return "static";
        case "position-try":                return "normal none";
        case "position-visibility":         return "anchors-visible";
        case "scroll-timeline":             return "none block";
        case "text-decoration-style":       return "solid";
        case "text-emphasis":               return "none currentcolor";
        case "text-overflow":               return "clip";
        case "transform-box":               return "view-box";
        case "transform-origin":            return "50% 50% 0";
        case "transform-style":             return "flat";
        case "transition-property":         return "all";
        case "user-modify":                 return "read-only";
        case "view-timeline":               return "none block auto";
        case "-webkit-border-before":       return "medium none canvastext";
        case "-webkit-mask-composite":      return "source-over";

        default:
          return null
      } break;

      case "revert":       information.value = "unset"; if (strict) return null;                      break; // ->> CSSOM prohibits user agent and user stylesheets access, so unable to rollback cascade layer from author origin
      case "revert-layer": cascaded = false; resolved = true; void unlayered.push(information.layer); break;

      case "unset": switch (property) {
        case "accent-color":
        case "border-collapse": case "border-spacing":
        case "caption-side":    case "caret-color": case "clip-rule": case "color": case "color-scheme": case "cursor":
        case "direction":       case "dominant-baseline":
        case "empty-cells":
        case "fill":                case "fill-opacity":        case "fill-rule":             case "font": case "font-family": case "font-feature-settings": case "font-kerning": case "font-language-override": case "font-optical-sizing": case "font-palette": case "font-size": case "font-size-adjust": case "font-smooth": case "font-stretch": case "font-style": case "font-synthesis": case "font-synthesis-position": case "font-synthesis-small-caps": case "font-synthesis-style": case "font-synthesis-weight": case "font-variant": case "font-variant-alternates": case "font-variant-caps": case "font-variant-east-asian": case "font-variant-emoji": case "font-variant-ligatures": case "font-variant-numeric": case "font-variant-position": case "font-variation-settings": case "font-weight": case "font-width": case "forced-color-adjust":
        case "hanging-punctuation": case "hyphenate-character": case "hyphenate-limit-chars": case "hyphens":
        case "image-orientation":   case "image-rendering":     case "image-resolution":      case "interpolate-size":
        case "letter-spacing":      case "line-break":          case "line-height":           case "line-height-step": case "list-style": case "list-style-image": case "list-style-position": case "list-style-type":
        case "marker":              case "marker-end":          case "marker-mid":            case "marker-start":     case "math-depth": case "math-shift":       case "math-style":
        case "orphans":             case "overflow-wrap":
        case "paint-order":         case "pointer-events": case "print-color-adjust":
        case "quotes":
        case "ruby-align":      case "ruby-overhang":   case "ruby-position":
        case "scrollbar-color": case "scrollbar-width": case "shape-rendering": case "speak":       case "speak-as":             case "stroke":                   case "stroke-dasharray":       case "stroke-dashoffset": case "stroke-linecap": case "stroke-linejoin":  case "stroke-miterlimit": case "stroke-opacity": case "stroke-width":
        case "tab-size":        case "text-align":      case "text-align-last": case "text-anchor": case "text-decoration-skip": case "text-decoration-skip-ink": case "text-emphasis-position": case "text-indent":       case "text-justify":   case "text-orientation": case "text-rendering":    case "text-shadow":    case "text-size-adjust": case "text-spacing-trim": case "text-transform": case "text-underline-offset": case "text-underline-position": case "text-wrap": case "text-wrap-mode": case "text-wrap-style":
        case "visibility":
        case "white-space":                 case "white-space-collapse":    case "widows":                case "word-break":          case "word-spacing":              case "writing-mode":
        case "-webkit-tap-highlight-color": case "-webkit-text-fill-color": case "-webkit-text-security": case "-webkit-text-stroke": case "-webkit-text-stroke-color": case "-webkit-text-stroke-width": case "-webkit-touch-callout":
          information.value = "inherit";
          break;

        default:
          information.value = "initial"
      } break;

      default:
        resolved = true
    }
  }

  return null === information || information.value === "" ? null : information.value
}

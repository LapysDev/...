var rule = {
  declarations: [
    {property: "margin", value: "0 !important"},
    {property: "padding", value: '0'}
  ],
  selector: "body"
};

/* ... */
function ruleToSource(rule) {
  return rule.selector + '{' + ruleDeclarationsToSource(rule.declarations) + '}'
}

function ruleDeclarationsToSource(declarations) {
  var source = "";

  for (var iterator = +0, length = declarations.length; iterator != length; ++iterator) {
    var declaration = declarations[iterator];
    source += declaration.property + ':' + declaration.value + ';'
  }

  return source
}

/* ... */
try {
  var head = document.getElementsByTagName("head")[0];
  var style = document.createElement("style");

  // ...
  style.innerHTML = ruleToSource(rule);
  if (null === head) {
    head = document.createElement("head");
    document.documentElement.appendChild(head)
  }

  // ...
  head.appendChild(style)
} catch (error) {
  if (document.styleSheets.length) {
    var stylesheet = document.styleSheets[0];

    if ("cssText" in stylesheet) stylesheet.cssText += ruleToSource(rule);
    else {
      var cssRuleCount;

      // ...
      if ("cssRules" in stylesheet) cssRuleCount = stylesheet.cssRules.length;
      else if ("rules" in stylesheet) cssRuleCount = stylesheet.rules.length;

      try {
        if ("function" === typeof(stylesheet.insertRule)) stylesheet.insertRule(ruleToSource(rule), cssRuleCount);
        else if ("function" === typeof(stylesheet.addRule)) stylesheet.addRule(rule.selector, ruleDeclarationsToSource(rule.declarations), cssRuleCount)
      } catch (error) {
        stylesheet.cssText += ruleToSource(rule)
      }
    }
  }
}

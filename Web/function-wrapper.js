// ->> Get the box 📦
function gift() { console.log("Here's your \"gift\" 🎁") };

// ->> — wrap it as a present 🎁
for (var count = (function getStackLimit() {
  var limit = 0;

  try { void function recurse() { ++limit, recurse() }() }
  catch (error) {}

  return limit
})() || 0x45 /* ->> nice */; count; --count) gift = (function(wrap /* ->> Persistent reference to the recent value of `gift` */) {
  return function() { console.log("Never gonna give you up… 🎶"), wrap() }
})(gift);

// ->> Open it! 🎉
gift()

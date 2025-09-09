void function paint() {
  var canvas  = document.createElement("canvas");
  var context = canvas.getContext("2d");
  var inputs  = [document.createElement("input"), document.createElement("input")];
  var height  = innerHeight;
  var width   = innerWidth;

  // ...
  function render(width, height) {
    var gradient = context.createLinearGradient(0.0, height * 0.3, width, height * 0.7);
    var mask     = "rgba(0, 0, 0, 0.85)";

    // ...
    canvas.style.height = (canvas.height = height) + "px";
    canvas.style.width  = (canvas.width  = width)  + "px";

    gradient.addColorStop(0.000, "#000000");
    gradient.addColorStop(0.475, "#0F0F0F");
    gradient.addColorStop(0.499, "#303030");
    gradient.addColorStop(0.500, "#909090");
    gradient.addColorStop(0.501, "#303030");
    gradient.addColorStop(0.525, "#0F0F0F");
    gradient.addColorStop(1.000, "#000000");

    context.fillStyle = gradient;
    context.fillRect(0.0, 0.0, width, height);

    context.fillStyle = mask;
    context.fillRect(0.0, 0.0, width, height);

    for (var count = Math.max(height, width); --count; ) {
      var y       = Math.trunc(Math.random() * height);
      var x       = Math.trunc(Math.random() * width);
      var size    = Math.trunc(Math.random() * 2.125);
      var opacity = ((Math.random() * 0.1) + ((1.0 - (x / width)) * 0.5) + ((1.0 - (y / height)) * 0.5)) / 2.0;

      // ...
      do {
        context.fillStyle = "rgba(255, 255, 255, " + opacity + ")";

        context.beginPath();
        context.arc(x, y, size, 0, Math.PI * 2.0);
        context.fill();
        context.closePath();

        opacity  = Math.max(1.0, opacity * 2.0);
        size    /= 2.0
      } while (size >= 1.0 && Math.random() > 0.5)
    }
  }

  // ...
  context.imageSmoothingEnabled = false;
  context.imageSmoothingQuality = "low";

  // ...
  while (document.body.childNodes.length)
  document.body.removeChild(document.body.childNodes.item(+0));

  // ...
  document.body.appendChild(canvas);
  with (document.body.style) {
    backgroundColor = "#333333";
    margin          = 0
  }

  for (var index = +0, length = inputs.length; index !== length; ++index) {
    var input = inputs[index];

    // ...
    document.body.appendChild(input);

    input.min      = 0;
    input.onchange = function() { input.value = Math.trunc(input.value); render(+inputs[0].value, +inputs[1].value) };
    input.oninput  = function() { input.value = Math.trunc(input.value); render(+inputs[0].value, +inputs[1].value) };
    input.step     = 1;
    input.type     = "number";
    with (input.style) {
      borderRadius  = 3 + "px";
      boxShadow     = "0 0 3px 3px rgba(127, 127, 127, 0.125)";
      color         = "#333333";
      fontFamily    = "\"Google Sans\", sans-serif, monospace";
      fontSize      = 16 + "px";
      fontWeight    = "lighter";
      left          = (((index + 0) * (innerWidth * 0.3)) + ((index + 1) * 10)) + "px";
      paddingBottom = 4 + "px";
      paddingLeft   = 5 + "px";
      paddingRight  = 5 + "px";
      paddingTop    = 4 + "px";
      position      = "absolute";
      top           = 90 + '%';
      width         = (innerWidth * 0.3) + "px"
    }

    // ...
    switch (index) {
      case 0: input.value = width;  break;
      case 1: input.value = height; break
    }
  }

  // ...
  render(width, height)
}()

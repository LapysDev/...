var canvas = document.createElement("canvas");
var context = canvas.getContext("2d");

var rotation = 0;
var size = 50;
var x = 0, y = 0;

// ...
canvas.style.backgroundColor = "#000000";
canvas.style.height          = (canvas.height = Math.min(innerHeight, innerWidth)) + "px";
canvas.style.width           = (canvas.width  = Math.min(innerHeight, innerWidth)) + "px";
context.fillStyle = "#FFFFFF";
document.body.style.margin = 0;

document.body.appendChild(canvas);

(function animate() {
  rotation = (rotation + 1) % 360;
  x = canvas.width / 2;
  ++y;

  context.clearRect(0, 0, canvas.width, canvas.height);
  context.translate(+x + (size / 2), +y + (size / 2));
  context.rotate(rotation * Math.PI / 180);
  context.fillRect(size / -2, size / -2, size, size);
  context.setTransform(1.0, 0.0, 0.0, 1.0, 0.0, 0.0);

  requestAnimationFrame(animate)
})()


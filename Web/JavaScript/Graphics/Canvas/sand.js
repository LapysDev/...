var canvas     = document.createElement("canvas");
var context    = canvas.getContext("2d");
var substance  = null;
var substances = null;

var GRAVITY    = 1.0;
var SUBSTANCES = {
  AIR  : {0: 0,   1: 0,   2: 0,   3: 255},
  SAND : {0: 244, 1: 164, 2: 96,  3: 255},
  STONE: {0: 169, 1: 169, 2: 169, 3: 255},

  detectable: []
};

// ...
function detect(x, y) {
  var index = (4 * x) + (4 * y * canvas.width);

  for (var iterator = SUBSTANCES.detectable.length; iterator; ) {
    var substance = SUBSTANCES.detectable[--iterator];
    if (
      substance[0] === substances[index + 0] &&
      substance[1] === substances[index + 1] &&
      substance[2] === substances[index + 2] &&
      substance[3] === substances[index + 3]
    ) return substance
  } return null
}

function place(SUBSTANCE) {
  substance.data[0] = SUBSTANCE[0];
  substance.data[1] = SUBSTANCE[1];
  substance.data[2] = SUBSTANCE[2];
  substance.data[3] = SUBSTANCE[3];
  return substance
}

// ...
canvas.style.backgroundColor = "#000000";
canvas.style.height = (canvas.height = innerHeight * 0.675) + "px";
canvas.style.margin = "16.25%";
canvas.style.width  = (canvas.width  = innerWidth  * 0.675) + "px";

context.imageSmoothingEnabled = false;
context.imageSmoothingQuality = "low";

document.body.style.backgroundColor = "#333333";
document.body.style.height = "100%";
document.body.style.margin = 0;
document.documentElement.style.height = "100%";

substance = context.createImageData(1, 1);
for (var type in SUBSTANCES) {
  if (SUBSTANCES.detectable !== SUBSTANCES[type])
  SUBSTANCES.detectable.push(SUBSTANCES[type])
}

// ...
document.body.appendChild(canvas);

// ...
onmousedown = function(event) {
  var canvasX = event.clientX - canvas.offsetLeft;
  var canvasY = event.clientY - canvas.offsetTop;
  var size = 20 + Math.trunc(Math.random() * 20);

  for (var y = size; y--; ) for (var x = size; x--; )
  context.putImageData(place(SUBSTANCES.STONE), canvasX + x - (size >> 1), canvasY + y - (size >> 1))
};

onmousemove = function(event) {
  var canvasX = event.clientX - canvas.offsetLeft;
  var canvasY = event.clientY - canvas.offsetTop;

  context.putImageData(place(SUBSTANCES.SAND), canvasX, canvasY)
};

// ...
void function setup() {
  for (var x = canvas.width; x; --x)
  for (var y = 5 + Math.trunc(Math.random() * 33); --y; y)
    context.putImageData(place(SUBSTANCES.SAND), x, y)
}();

void function simulate() {
  var height = canvas.height;
  var width  = canvas.width;

  substances = context.getImageData(0, 0, canvas.width, canvas.height).data;
  for (var index = (4 * height * width) - 4; index !== -4; index -= 4) {
    var x = Math.trunc((index / 4) % width);
    var y = Math.trunc((index / 4) / width);

    switch (detect(x, y)) {
      case SUBSTANCES.SAND: {
        switch (detect(x, y + 1)) {
          case SUBSTANCES.STONE: {
            context.putImageData(place(SUBSTANCES.AIR), x, y);
            context.putImageData(place(SUBSTANCES.SAND), x - 1, y)
          } break;

          default: {
            context.putImageData(place(SUBSTANCES.AIR), x, y);
            context.putImageData(place(SUBSTANCES.SAND), x, y + GRAVITY)
          }
        }
      } break
    }
  }

  requestAnimationFrame(simulate)
}()

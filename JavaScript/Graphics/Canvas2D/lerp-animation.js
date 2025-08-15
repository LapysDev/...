var {PI, max, min, random} = Math;

var bounds        = {height: 200, width: 200, x: 0, y: 0};
var canvas        = document.createElement("canvas");
var context       = canvas.getContext("2d");
var frame         = 0, frameId;
var interpolating = true;         // → Animate (rather than jitter) the movement
var stuttering    = true;         // → Move the drawing per frame
var translation   = {x: 0, y: 0}; // → Movement location for drawing
var viewport      = {dpp: devicePixelRatio, fps: 60, height: innerHeight, width: innerWidth};

// …
function resize() {
  viewport.height            = innerHeight;
  viewport.width             = innerWidth;
  document.body.style.margin = 0;
  canvas.height              = viewport.height;
  canvas.style.height        = viewport.height + "px";
  canvas.style.width         = viewport.width  + "px";
  canvas.width               = viewport.width
}

void function start() {
  bounds.x                = random() * max(0, viewport.width  - bounds.width);
  bounds.y                = random() * max(0, viewport.height - bounds.height);
  document.body.innerText = "";
  viewport.height        *= viewport.dpp;
  viewport.width         *= viewport.dpp;

  cancelAnimationFrame(frameId);
  resize();
  document.body.append(canvas);
  window.addEventListener("resize", resize)
}();

void function update() {
  function clamp(value, minimum, maximum) { return min(maximum, max(minimum, value)) }
  function lerp(start, end, time) { return ((1.0 - time) * start) + ((0.0 + time) * end) }
  function rand(size) { return (u32) (random() * size) * (random() > 0.5 ? +1 : -1) }
  function u32(number) { return number >>> 0 }

  // …
  context.fillStyle = "rgb(" + (u32) (255.0 * (bounds.x / viewport.width)) + ", 120, " + (u32) (255.0 * (bounds.y / viewport.height)) + ')';

  context.clearRect(0, 0, viewport.dpp * viewport.width, viewport.height);
  context.beginPath();
  context.ellipse(bounds.x, bounds.y, bounds.width * 0.5, bounds.height * 0.5, 0.0, 0.0, PI * 2.0, false);
  context.fill();

  // … → Every ⅒ second
  if (0 === frame++ % (u32) (viewport.fps * 0.1)) {
    // translation.x = clamp(stuttering ? bounds.x + rand(50) : bounds.x, 0, viewport.width  - bounds.width);
    // translation.y = clamp(stuttering ? bounds.y + rand(50) : bounds.y, 0, viewport.height - bounds.height)
    // → or
    translation.x = clamp(stuttering ? (u32) (viewport.width  * random()) : bounds.x, 0, viewport.width  - bounds.width);
    translation.y = clamp(stuttering ? (u32) (viewport.height * random()) : bounds.y, 0, viewport.height - bounds.height)
  }

  bounds.x = interpolating ? lerp(bounds.x, translation.x, 0.2) : translation.x;
  bounds.y = interpolating ? lerp(bounds.y, translation.y, 0.1) : translation.y;
  frameId  = requestAnimationFrame(update)
}()

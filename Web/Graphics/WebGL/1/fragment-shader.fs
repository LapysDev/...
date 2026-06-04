// Directive > ...
precision mediump float;

// Initialization > Varying Color
varying vec3 VARYING_COLOR;

// Main
void main() {
    // Update > (Graphics Library > Fragment Color)
    gl_FragColor = vec4(VARYING_COLOR, 1);
}

// Directive > ...
precision mediump float;

// Initialization
    // Matrix Transformation
    uniform mat4 TRANSFORMATION_MATRIX;

    // Position
    attribute vec3 POSITION;

    // Varying Color
    varying vec3 VARYING_COLOR;

// Main
void main() {
    // Update > ((Graphics Library > Position), Varying Color)
    gl_Position = TRANSFORMATION_MATRIX * vec4(POSITION, 1);
    VARYING_COLOR = vec3(POSITION.xy, 1);
}

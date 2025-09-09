// Directive > ...
precision mediump float;

// Initialization
    // Color, Position
    attribute vec3 COLOR;
    attribute vec3 POSITION;

    // Matrix Transformation
    uniform mat4 TRANSFORMATION_MATRIX;

    // Varying Color
    varying vec3 VARYING_COLOR;

// Main
void main() {
    // Update > ((Graphics Library > Position), Varying Color)
    gl_Position = TRANSFORMATION_MATRIX * vec4(POSITION, 1);
    VARYING_COLOR = COLOR;
}

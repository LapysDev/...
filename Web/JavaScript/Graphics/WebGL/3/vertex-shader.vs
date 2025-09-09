// Directive > ...
precision mediump float;

// Initialization
    // Matrix Transformation
    uniform mat4 TRANSFORMATION_MATRIX;

    // Position, Texture
    attribute vec3 POSITION;
    attribute vec2 TEXTURE;

    // Varying Texture
    varying vec2 VARYING_TEXTURE;

// Main
void main() {
    // Update > ((Graphics Library > Position), Varying Texture)
    gl_Position = TRANSFORMATION_MATRIX * vec4(POSITION, 1);
    VARYING_TEXTURE = TEXTURE;
}

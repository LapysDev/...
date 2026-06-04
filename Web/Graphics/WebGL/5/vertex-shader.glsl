/* Directive */
precision mediump float;

/* Global */
    // Fragment (Color, Texture Position)
    varying vec4 FRAGMENT_COLOR;
    varying vec2 FRAGMENT_TEXTURE_POSITION;

    // (Projection, View, World) Matrix
    uniform mat4 PROJECTION_MATRIX, VIEW_MATRIX, WORLD_MATRIX;

    // Vertex (Color, (Texture) Position)
    attribute vec4 VERTEX_COLOR;
    attribute vec3 VERTEX_POSITION;
    attribute vec2 VERTEX_TEXTURE_POSITION;

/* Main */
void main() {
    // Update > Fragment (Color, Texture Position)
    FRAGMENT_COLOR = VERTEX_COLOR;
    FRAGMENT_TEXTURE_POSITION = VERTEX_TEXTURE_POSITION;

    // Update > (Graphics Library > Position)
    gl_Position = PROJECTION_MATRIX * VIEW_MATRIX * WORLD_MATRIX * vec4(VERTEX_POSITION, 1.0);
}

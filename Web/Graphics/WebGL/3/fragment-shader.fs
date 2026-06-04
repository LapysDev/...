// Directive > ...
precision mediump float;

// Initialization
    // 2D Sampler
    uniform sampler2D TEXTURE_ID;

    // Varying Texture
    varying vec2 VARYING_TEXTURE;

// Main
void main() {
    // Update > (Graphics Library > Color)
    gl_FragColor = texture2D(TEXTURE_ID, VARYING_TEXTURE);
}

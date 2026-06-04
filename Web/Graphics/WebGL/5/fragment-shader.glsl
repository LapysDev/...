/* Directive */
precision mediump float;

/* Global */
    // Fragment (Color, Texture Position)
    varying vec4 FRAGMENT_COLOR;
    varying vec2 FRAGMENT_TEXTURE_POSITION;

    // Fragment Sampler
    uniform sampler2D FRAGMENT_SAMPLER;

/* Main */
void main() {
    // Initialization > Color
    vec4 COLOR = texture2D(FRAGMENT_SAMPLER, FRAGMENT_TEXTURE_POSITION);

    // Update > Color
    COLOR.xyz = ((COLOR.xyz * abs(COLOR.w - FRAGMENT_COLOR.w)) + (FRAGMENT_COLOR.xyz * FRAGMENT_COLOR.w));

    // Update > (Graphics Library > Fragment Color)
    gl_FragColor = COLOR;
}

// Directive > ...
precision mediump float;

// Initialization
    // Texture ID
    uniform sampler2D TEXTURE_ID;

    // Varying (Brightness, Texture)
    varying float VARYING_BRIGHTNESS;
    varying vec2 VARYING_TEXTURE;

/* Main */
void main() {
    // Initialization > Color
    vec4 color = texture2D(TEXTURE_ID, VARYING_TEXTURE);

    // : Update > Color --- NOTE (Lapys) -> Modify the first three components (i.e.: X, Y and Z) which are representative of an RGB (not RGBA) sequence.
    // : Update > (Graphics Library > Fragment Color)
    color.xyz *= VARYING_BRIGHTNESS;
    gl_FragColor = color;
}

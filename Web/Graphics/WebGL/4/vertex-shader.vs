// Directive > ...
precision mediump float;

// Initialization
    // Light Direction --- NOTE (Lapys) -> Above and ahead of the camera.
    const float AMBIENT_LIGHT = 0.1;
    const vec3 LIGHT_DIRECTION = normalize(vec3(0.0, 1.0, 1.0));

    // (Normal, Transformation) Matrix
    uniform mat4 NORMAL_MATRIX;
    uniform mat4 TRANSFORMATION_MATRIX;

    // Normal, Position, Texture
    attribute vec3 NORMAL;
    attribute vec3 POSITION;
    attribute vec2 TEXTURE;

    // Varying (Brightness, Texture)
    varying float VARYING_BRIGHTNESS;
    varying vec2 VARYING_TEXTURE;

/* Main */
void main() {
    // Initialization > World Normal --- NOTE (Lapys) -> Direction of the "Camera" regardless of transformations.
    vec3 worldNormal = (NORMAL_MATRIX * vec4(NORMAL, 1.0)).xyz;

    // Update > (Graphics Library > Position)
    // : Update > ...
    gl_Position = TRANSFORMATION_MATRIX * vec4(POSITION, 1.0);

    VARYING_BRIGHTNESS = max(0.0, dot(worldNormal, LIGHT_DIRECTION)) + AMBIENT_LIGHT;
    VARYING_TEXTURE = TEXTURE;
}

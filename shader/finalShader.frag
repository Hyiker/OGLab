#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D screenTexture;
uniform sampler2D lightDepthTexture;
uniform sampler2D bloomTexture;

vec3 reinhardToneMapping(vec3 color, float adaptedLum) {
    const float MIDDLE_GREY = 1;
    color *= MIDDLE_GREY / adaptedLum;
    return color / (1.0f + color);
}
vec3 ACESToneMapping(vec3 color, float adaptedLum) {
    const float A = 2.51f;
    const float B = 0.03f;
    const float C = 2.43f;
    const float D = 0.59f;
    const float E = 0.14f;

    color *= adaptedLum;
    return (color * (A * color + B)) / (color * (C * color + D) + E);
}

vec3 F(vec3 x) {
    const float A = 0.22f;
    const float B = 0.30f;
    const float C = 0.10f;
    const float D = 0.20f;
    const float E = 0.01f;
    const float F = 0.30f;

    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

vec3 Uncharted2ToneMapping(vec3 color, float adaptedLum) {
    const float WHITE = 11.2;
    return F(1.6 * adaptedLum * color) / F(vec3(WHITE));
}
void main() {
    vec3 color = texture(screenTexture, texCoord).rgb;
    vec3 bloomColor = texture(bloomTexture, texCoord).rgb;

    color += bloomColor;

    vec3 colorDbg = color;
    vec3 depth = texture(lightDepthTexture, texCoord).rrr;

    color = ACESToneMapping(color, 2.0);

    // gamma correction
    // ! FINAL STEP
    float gamma = 2.2;
    color = pow(color, vec3(1.0 / gamma));

    // FragColor = vec4(colorDbg, 1.0);
    FragColor = vec4(color, 1.0);
}
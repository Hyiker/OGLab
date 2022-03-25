#version 330 core

layout(location = 0) out vec4 oBlurred;

in vec2 texCoord;

uniform sampler2D uSrcTex;
uniform vec2 uDirection;

vec4 blur5(sampler2D image, vec2 uv, vec2 direction) {
    vec4 color = vec4(0.0);
    vec2 off1 = vec2(1.3333333333333333) * direction;
    vec2 resolution = textureSize(uSrcTex, 0);
    color += texture(image, uv) * 0.29411764705882354;
    color += texture(image, uv + (off1 / resolution)) * 0.35294117647058826;
    color += texture(image, uv - (off1 / resolution)) * 0.35294117647058826;
    return color;
}

void main() { oBlurred = blur5(uSrcTex, texCoord, uDirection); }
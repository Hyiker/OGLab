#version 330 core

uniform sampler2D uInputTexture;
in vec2 texCoord;

out vec4 color;
void main() {
    vec3 inputColor = texture(uInputTexture, texCoord).rgb;
    float brightness = dot(inputColor, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0)
        color = vec4(inputColor, 3.0);
    else
        color = vec4(0.0);
}
#version 330

layout(location = 0) in vec3 aPosition;

uniform mat4 uLightTransform;
uniform mat4 uModel;

void main(void) {
    gl_Position = uLightTransform * uModel * vec4(aPosition, 1.0);
}

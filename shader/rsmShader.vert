#version 330

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 uModel;
uniform mat4 uLightTransform;
uniform mat4 uNormalTransform;

out vec3 position;
out vec3 normal;
out vec2 texCoord;

void main(void) {
    position = (uModel * vec4(aPosition, 1.0)).xyz;
    texCoord = aTexCoord;
    normal = vec3(uNormalTransform * vec4(aNormal, 0.0));
    gl_Position = uLightTransform * vec4(position, 1.0);
}

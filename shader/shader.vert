#version 330

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normalTransform;

out vec4 position;
out vec3 normal;
out vec2 texCoord;

void main(void) {
    position = view * model * vec4(aPosition, 1.0);

    normal = vec3(normalTransform * vec4(aNormal, 0.0));
    texCoord = aTexCoord;

    gl_Position = projection * position;
}

#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normalTransform;

out vec4 fPosition;
out vec3 fNormal;

void main(void) {
    fPosition = view * model * vec4(position, 1.0);

    fNormal = vec3(normalTransform * vec4(normal, 0.0));

    gl_Position = projection * fPosition;
    /*gl_Position.x *= 1000.0f;*/
    /*gl_Position.y = 0.0;*/
}

#version 330 core
layout(location = 0) out vec3 rsmPosition;
layout(location = 1) out vec3 rsmNormal;
layout(location = 2) out vec3 rsmFlux;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

struct Material {
    vec3 diffuse;

    sampler2D diffuseTex;
};

uniform Material material;
uniform vec3 uLightDir;

void main() {
    rsmPosition = position;
    rsmNormal = normalize(normal);
    rsmFlux = vec3(5, 4, 4) * max(dot(normalize(-uLightDir), rsmNormal), 0.0) *
              texture(material.diffuseTex, texCoord).rgb * material.diffuse;
}
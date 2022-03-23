#version 330 core
layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

struct Material {
    sampler2D diffuseTex;
    sampler2D specularTex;
};

uniform Material material;

void main() {
    gPosition = position;
    gNormal = normalize(normal);
    gAlbedoSpec.rgb = texture(material.diffuseTex, texCoord).rgb;
    gAlbedoSpec.a = texture(material.specularTex, texCoord).r;
}
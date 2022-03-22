#version 330

in vec4 position;
in vec3 normal;
in vec2 texCoord;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
    float ior;
    int illum;

    sampler2D ambientTex;
    sampler2D diffuseTex;
    sampler2D specularTex;
    sampler2D displacementTex;
};

uniform Material material;

// output
out vec4 color;

float mipmapLevel(in vec2 texCoord) {
    vec2 a = dFdx(texCoord);
    vec2 b = dFdy(texCoord);
    float L = max(length(a), length(b));
    return abs(log2(L));
}

void main(void) {
    int roundedD = int(mipmapLevel(texCoord));
    color = vec4(float(roundedD) / 10.0);
    /*color = vec3(1,0,0);*/
}

#version 330

in vec3 position;
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
};

uniform Material material;
uniform vec3 sunPosition;
uniform vec3 camPosition;

// output
out vec4 color;

void main(void) {
    float distance = length(sunPosition - position);
    float attenuation = 1.0 / distance;

    vec3 ambient =
        material.ambient * texture(material.ambientTex, texCoord).rgb;

    vec3 lightDir = normalize(sunPosition - position);
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * material.diffuse *
                   texture(material.diffuseTex, texCoord).rgb;

    vec3 viewDir = normalize(camPosition - position);
    vec3 H = normalize(viewDir + lightDir);
    vec3 specular = material.specular *
                    texture(material.specularTex, texCoord).rgb *
                    pow(max(dot(H, normal), 0.0), material.shininess);

    color = vec4((ambient + diffuse + specular) * attenuation, 1.0);
    /*color = vec3(1,0,0);*/
}

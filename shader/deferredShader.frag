#version 330

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D screenTexture;

struct GBuffer {
    sampler2D positionTex;
    sampler2D depthTex;
    sampler2D normalTex;
    sampler2D albedoTex;
};

uniform GBuffer uGbuffer;
uniform vec3 uLightDir;
uniform vec3 uCamPosition;

// output
out vec4 color;

#define N_SAMPLE 100
#define N_SAMPLE_INV 0.01

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

#define PI 3.14159265359
#define PI_INV 0.31830989

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 brdf(in vec3 N, in vec3 wi, in vec3 wo, in float cosTheta, in vec3 F) {
    vec3 res = vec3(0.0);
    vec3 H = normalize(wi + wo);
    float roughness = 0.5;

    // normal distribution
    float NDF = DistributionGGX(N, H, roughness);
    // geometry occlusion
    float G = GeometrySmith(N, wo, wi, roughness);

    vec3 nominator = NDF * G * F;
    float denominator =
        4.0 * max(dot(N, wo), 0.0) * max(dot(N, wi), 0.0) + 0.001;
    res = nominator / denominator;
    return res;
}
vec3 sampleLightDir() { return normalize(-uLightDir); }
vec3 calcIrradiance(in vec3 N, in vec3 viewDir, in vec3 albedo) {
    vec3 Lo = vec3(0.0);
    float dW = N_SAMPLE_INV;
    vec3 wo = normalize(viewDir);
    vec3 wi = sampleLightDir();
    float cosTheta = max(0.0, dot(wi, N));
    vec3 F = fresnelSchlick(cosTheta, vec3(0.04));

    vec3 kSpecular = F;
    vec3 kDiffuse = vec3(1.0) - kSpecular;

    vec3 specular = brdf(N, wi, wo, cosTheta, F);
    vec3 radiance = vec3(1.0) * 5.0;
    Lo += (kDiffuse * albedo * PI_INV + specular) * radiance * dot(N, wi);

    return Lo;
}

void main(void) {
    vec3 position = texture(uGbuffer.positionTex, texCoord).rgb;
    vec3 albedo = texture(uGbuffer.albedoTex, texCoord).rgb;
    vec3 normal = texture(uGbuffer.normalTex, texCoord).xyz;
    float depth = texture(uGbuffer.depthTex, texCoord).r;

    vec3 lightDir = normalize(-uLightDir);
    vec3 viewDir = normalize(uCamPosition - position);

    vec3 irradiance = calcIrradiance(normal, viewDir, albedo);

    color = vec4(irradiance, 1.0);
}
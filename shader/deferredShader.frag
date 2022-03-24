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
struct ShadowMap {
    sampler2D depthMap;
    mat4 lightTransform;
};

uniform GBuffer uGbuffer;
uniform ShadowMap uShadowMap;
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
    vec3 radiance = vec3(3.0) * 5.0;
    Lo += (kDiffuse * albedo * PI_INV + specular) * radiance * dot(N, wi);

    return Lo;
}

vec2 poissonDisk[64];
void initPoissonDisk() {
    poissonDisk[0] = vec2(-0.613392, 0.617481);
    poissonDisk[1] = vec2(0.170019, -0.040254);
    poissonDisk[2] = vec2(-0.299417, 0.791925);
    poissonDisk[3] = vec2(0.645680, 0.493210);
    poissonDisk[4] = vec2(-0.651784, 0.717887);
    poissonDisk[5] = vec2(0.421003, 0.027070);
    poissonDisk[6] = vec2(-0.817194, -0.271096);
    poissonDisk[7] = vec2(-0.705374, -0.668203);
    poissonDisk[8] = vec2(0.977050, -0.108615);
    poissonDisk[9] = vec2(0.063326, 0.142369);
    poissonDisk[10] = vec2(0.203528, 0.214331);
    poissonDisk[11] = vec2(-0.667531, 0.326090);
    poissonDisk[12] = vec2(-0.098422, -0.295755);
    poissonDisk[13] = vec2(-0.885922, 0.215369);
    poissonDisk[14] = vec2(0.566637, 0.605213);
    poissonDisk[15] = vec2(0.039766, -0.396100);
    poissonDisk[16] = vec2(0.751946, 0.453352);
    poissonDisk[17] = vec2(0.078707, -0.715323);
    poissonDisk[18] = vec2(-0.075838, -0.529344);
    poissonDisk[19] = vec2(0.724479, -0.580798);
    poissonDisk[20] = vec2(0.222999, -0.215125);
    poissonDisk[21] = vec2(-0.467574, -0.405438);
    poissonDisk[22] = vec2(-0.248268, -0.814753);
    poissonDisk[23] = vec2(0.354411, -0.887570);
    poissonDisk[24] = vec2(0.175817, 0.382366);
    poissonDisk[25] = vec2(0.487472, -0.063082);
    poissonDisk[26] = vec2(-0.084078, 0.898312);
    poissonDisk[27] = vec2(0.488876, -0.783441);
    poissonDisk[28] = vec2(0.470016, 0.217933);
    poissonDisk[29] = vec2(-0.696890, -0.549791);
    poissonDisk[30] = vec2(-0.149693, 0.605762);
    poissonDisk[31] = vec2(0.034211, 0.979980);
    poissonDisk[32] = vec2(0.503098, -0.308878);
    poissonDisk[33] = vec2(-0.016205, -0.872921);
    poissonDisk[34] = vec2(0.385784, -0.393902);
    poissonDisk[35] = vec2(-0.146886, -0.859249);
    poissonDisk[36] = vec2(0.643361, 0.164098);
    poissonDisk[37] = vec2(0.634388, -0.049471);
    poissonDisk[38] = vec2(-0.688894, 0.007843);
    poissonDisk[39] = vec2(0.464034, -0.188818);
    poissonDisk[40] = vec2(-0.440840, 0.137486);
    poissonDisk[41] = vec2(0.364483, 0.511704);
    poissonDisk[42] = vec2(0.034028, 0.325968);
    poissonDisk[43] = vec2(0.099094, -0.308023);
    poissonDisk[44] = vec2(0.693960, -0.366253);
    poissonDisk[45] = vec2(0.678884, -0.204688);
    poissonDisk[46] = vec2(0.001801, 0.780328);
    poissonDisk[47] = vec2(0.145177, -0.898984);
    poissonDisk[48] = vec2(0.062655, -0.611866);
    poissonDisk[49] = vec2(0.315226, -0.604297);
    poissonDisk[50] = vec2(-0.780145, 0.486251);
    poissonDisk[51] = vec2(-0.371868, 0.882138);
    poissonDisk[52] = vec2(0.200476, 0.494430);
    poissonDisk[53] = vec2(-0.494552, -0.711051);
    poissonDisk[54] = vec2(0.612476, 0.705252);
    poissonDisk[55] = vec2(-0.578845, -0.768792);
    poissonDisk[56] = vec2(-0.772454, -0.090976);
    poissonDisk[57] = vec2(0.504440, 0.372295);
    poissonDisk[58] = vec2(0.155736, 0.065157);
    poissonDisk[59] = vec2(0.391522, 0.849605);
    poissonDisk[60] = vec2(-0.620106, -0.328104);
    poissonDisk[61] = vec2(0.789239, -0.419965);
    poissonDisk[62] = vec2(-0.545396, 0.538133);
    poissonDisk[63] = vec2(-0.178564, -0.596057);
}
#define N_POISSON_SAMPLE 64
vec2 samplePoissonDisk(int i) { return poissonDisk[i]; }

#define EPS 5e-3
#define EPS_10 5e-2
float calcShadow(in vec3 position, in ShadowMap shadowMap, in vec3 N, in vec3 L,
                 inout vec3 dbg) {
    vec4 lightSpacePos = (shadowMap.lightTransform * vec4(position, 1.0));
    vec3 coord = lightSpacePos.xyz / lightSpacePos.w;
    coord = coord + vec3(1.0);
    coord *= 0.5;

    float sampleDepth = coord.z;

    vec2 texSize = textureSize(shadowMap.depthMap, 0);
    vec2 sampleStepSize = vec2(4.0);
    vec2 sampleSize = sampleStepSize / texSize;
    int n_visible = 0;
    float bias = max(EPS_10 * (1.0 - dot(N, L)), EPS);
    for (int i = 0; i < N_POISSON_SAMPLE; i++) {
        float shadwoMapDepth =
            texture(shadowMap.depthMap,
                    coord.xy + sampleSize * samplePoissonDisk(i))
                .r;

        n_visible += shadwoMapDepth >= sampleDepth - bias ? 1 : 0;
    }
    // dbg.r = shadwoMapDepth;
    dbg.g = sampleDepth;
    return float(n_visible) / float(N_POISSON_SAMPLE);
}

void main(void) {
    initPoissonDisk();
    vec3 position = texture(uGbuffer.positionTex, texCoord).xyz;
    vec3 albedo = texture(uGbuffer.albedoTex, texCoord).rgb;
    vec3 normal = texture(uGbuffer.normalTex, texCoord).xyz;
    float depth = texture(uGbuffer.depthTex, texCoord).r;

    vec3 lightDir = normalize(-uLightDir);
    vec3 viewDir = normalize(uCamPosition - position);

    vec3 shadowDbg;
    float visibility =
        calcShadow(position, uShadowMap, normal, lightDir, shadowDbg);
    vec3 irradiance = calcIrradiance(normal, viewDir, albedo);

    color = vec4(visibility * irradiance, 1.0);
}
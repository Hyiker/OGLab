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
struct RSMBuffer {
    sampler2D positionTex;
    sampler2D depthTex;
    sampler2D normalTex;
    sampler2D fluxTex;
};

uniform GBuffer uGbuffer;
uniform ShadowMap uShadowMap;
uniform vec3 uLightDir;
uniform vec3 uCamPosition;
uniform RSMBuffer uRSMBuffer;

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
uniform int uRSMRandomTexSize;
uniform sampler2D uRSMRandomTex;
uniform float uIndirIntensity;
// from
// https://sudonull.com/post/70-Reflective-Shadow-Maps-Part-2-Implementation
vec3 doReflectiveShadowMapping(vec3 P, vec3 N, vec3 lCoord) {
    vec3 indirectIllumination = vec3(0, 0, 0);
    float rMax = 0.03;
    for (int i = 0; i < uRSMRandomTexSize; ++i) {
        vec3 rnd = texelFetch(uRSMRandomTex, ivec2(i, 0), 0).xyz;
        vec2 coords = lCoord.xy + rMax * rnd.xy;
        vec3 vplPositionWS = texture(uRSMBuffer.positionTex, coords).xyz;
        vec3 vplNormalWS = normalize(texture(uRSMBuffer.normalTex, coords).xyz);
        vec3 flux = texture(uRSMBuffer.fluxTex, coords).rgb;
        vec3 r = P - vplPositionWS;
        vec3 result =
            flux * ((max(0, dot(vplNormalWS, r)) * max(0, dot(N, -r))) /
                    pow(length(r), 4));
        result *= rnd.z;
        indirectIllumination += result;
    }
    return max(indirectIllumination, vec3(0.0)) / float(uRSMRandomTexSize);
}

vec3 calcIrradiance(in vec3 N, in vec3 viewDir, in vec3 albedo,
                    in vec3 radiance) {
    vec3 Lo = vec3(0.0);
    vec3 wo = normalize(viewDir);
    vec3 wi = sampleLightDir();
    float cosTheta = max(0.0, dot(wi, N));
    vec3 F = fresnelSchlick(cosTheta, vec3(0.04));

    vec3 kSpecular = F;
    vec3 kDiffuse = vec3(1.0) - kSpecular;

    vec3 specular = brdf(N, wi, wo, cosTheta, F);
    Lo += (kDiffuse * albedo + specular) * radiance * dot(N, wi) * PI_INV;

    return Lo;
}

vec4 convertOptimizedMoments(in vec4 optimizedMoments) {
    optimizedMoments[0] -= 0.03595588480;
    return mat4(0.222774414, 0.154967926, 0.145198894, 0.16312744, 0.077197286,
                0.139462942, 0.212020215, 0.259143226, 0.792698663, 0.796341583,
                0.725869446, 0.653909249, 0.031941755, -0.172282317,
                -0.275801481, -0.337613173) *
           optimizedMoments;
}

float mad(in float mvalue, in float avalue, in float bvalue) {
    return mvalue * avalue + bvalue;
}

float compute_msm_shadow_intensity(vec4 b, float fragment_depth) {
    // OpenGL 4 only - fma has higher precision:
    // float l32_d22 = fma(-b.x, b.y, b.z); // a * b + c
    // float d22 = fma(-b.x, b.x, b.y);     // a * b + c
    // float squared_depth_variance = fma(-b.x, b.y, b.z); // a * b + c

    float l32_d22 = -b.x * b.y + b.z;
    float d22 = -b.x * b.x + b.y;
    float squared_depth_variance = -b.x * b.y + b.z;

    float d33_d22 =
        dot(vec2(squared_depth_variance, -l32_d22), vec2(d22, l32_d22));
    float inv_d22 = 1.0 - d22;
    float l32 = l32_d22 * inv_d22;

    float z_zero = fragment_depth;
    vec3 c = vec3(1.0, z_zero - b.x, z_zero * z_zero);
    c.z -= b.y + l32 * c.y;
    c.y *= inv_d22;
    c.z *= d22 / d33_d22;
    c.y -= l32 * c.z;
    c.x -= dot(c.yz, b.xy);

    float inv_c2 = 1.0 / c.z;
    float p = c.y * inv_c2;
    float q = c.x * inv_c2;
    float r = sqrt((p * p * 0.25) - q);

    float z_one = -p * 0.5 - r;
    float z_two = -p * 0.5 + r;

    vec4 switch_msm;
    if (z_two < z_zero) {
        switch_msm = vec4(z_one, z_zero, 1.0, 1.0);
    } else {
        if (z_one < z_zero) {
            switch_msm = vec4(z_zero, z_one, 0.0, 1.0);
        } else {
            switch_msm = vec4(0.0);
        }
    }

    float quotient =
        (switch_msm.x * z_two - b.x * (switch_msm.x + z_two + b.y)) /
        ((z_two - switch_msm.y) * (z_zero - z_one));
    return clamp(switch_msm.y + switch_msm.z * quotient, 0.0, 1.0);
}
// https://github.com/TheRealMJP/Shadows/blob/master/Shadows/MSM.hlsl
float computeMSMHamburger(in vec4 moments, in float fragmentDepth,
                          in float depthBias, in float momentBias) {
    // Bias input data to avoid artifacts
    vec4 b = mix(moments, vec4(0.5, 0.5, 0.5, 0.5), momentBias);
    vec4 z;
    z[0] = fragmentDepth - depthBias;

    // Compute a Cholesky factorization of the Hankel matrix B storing only non-
    // trivial entries or related products
    float L32D22 = mad(-b[0], b[1], b[2]);
    float D22 = mad(-b[0], b[0], b[1]);
    float squaredDepthVariance = mad(-b[1], b[1], b[3]);
    float D33D22 = dot(vec2(squaredDepthVariance, -L32D22), vec2(D22, L32D22));
    float InvD22 = 1.0f / D22;
    float L32 = L32D22 * InvD22;

    // Obtain a scaled inverse image of bz = (1,z[0],z[0]*z[0])^T
    vec3 c = vec3(1.0f, z[0], z[0] * z[0]);

    // Forward substitution to solve L*c1=bz
    c[1] -= b.x;
    c[2] -= b.y + L32 * c[1];

    // Scaling to solve D*c2=c1
    c[1] *= InvD22;
    c[2] *= D22 / D33D22;

    // Backward substitution to solve L^T*c3=c2
    c[1] -= L32 * c[2];
    c[0] -= dot(c.yz, b.xy);

    // Solve the quadratic equation c[0]+c[1]*z+c[2]*z^2 to obtain solutions
    // z[1] and z[2]
    float p = c[1] / c[2];
    float q = c[0] / c[2];
    float D = (p * p * 0.25f) - q;
    float r = sqrt(D);
    z[1] = -p * 0.5f - r;
    z[2] = -p * 0.5f + r;

    // Compute the shadow intensity by summing the appropriate weights
    vec4 switchVal = (z[2] < z[0])
                         ? vec4(z[1], z[0], 1.0f, 1.0f)
                         : ((z[1] < z[0]) ? vec4(z[0], z[1], 0.0f, 1.0f)
                                          : vec4(0.0f, 0.0f, 0.0f, 0.0f));
    float quotient =
        (switchVal[0] * z[2] - b[0] * (switchVal[0] + z[2]) + b[1]) /
        ((z[2] - switchVal[1]) * (z[0] - z[1]));
    float shadowIntensity = switchVal[2] + switchVal[3] * quotient;
    return 1.0f - clamp(shadowIntensity, 0.0, 1.0);
}

#define EPS 5e-3
#define EPS_10 5e-2
float calcShadow(in vec3 position, in ShadowMap shadowMap, in vec3 N, in vec3 L,
                 inout vec3 dbg, in vec3 coord) {
    float sampleDepth = coord.z;

    vec4 moments =
        convertOptimizedMoments(texture(shadowMap.depthMap, coord.xy));

    int n_visible = 0;
    float bias = max(EPS_10 * (1.0 - dot(N, L)), EPS);
    // dbg.r = shadwoMapDepth;
    dbg.g = sampleDepth;
    float shadow = computeMSMHamburger(moments, sampleDepth, bias, 3e-5);
    shadow = smoothstep(0.7, 1.0, shadow);
    return shadow;
}

#define ZNEAR 0.01
#define ZFAR 50.0
float linearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * ZNEAR * ZFAR) / (ZFAR + ZNEAR - z * (ZFAR - ZNEAR));
}
uniform sampler2D uRandomHemisphereTex;
uniform vec3 uRandomSamples[64];
uniform mat4 uProjection;
uniform mat4 uView;
const vec2 noiseScale = vec2(1280.0 / 4.0, 960.0 / 4.0);
float calcSSAO(in vec3 position, in vec3 normal) {
    vec3 randomVec = texture(uRandomHemisphereTex, noiseScale * texCoord).xyz;
    vec3 viewSpacePosition = (uView * vec4(position, 1.0)).xyz;

    // rotate TBN matrix by randomvec texture
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    for (int i = 0; i < 64; ++i) {
        vec3 sample = TBN * uRandomSamples[i];
        sample = viewSpacePosition + sample * 1.0;

        vec4 offset = vec4(sample, 1.0);
        offset = uProjection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float sampleDepth =
            linearizeDepth(texture(uGbuffer.depthTex, offset.xy).r);

        // range check & accumulate
        float rangeCheck =
            smoothstep(0.0, 1.0, 1.0 / abs(viewSpacePosition.z + sampleDepth));

        occlusion += (EPS <= sampleDepth + sample.z ? 1.0 : 0.0) * rangeCheck;
    }
    return 1.0 - (occlusion / 64.0);
}

void main(void) {
    vec3 position = texture(uGbuffer.positionTex, texCoord).xyz;
    vec3 albedo = texture(uGbuffer.albedoTex, texCoord).rgb;
    vec3 normal = texture(uGbuffer.normalTex, texCoord).xyz;
    float depth = texture(uGbuffer.depthTex, texCoord).r;

    vec3 lightDir = normalize(-uLightDir);
    vec3 viewDir = normalize(uCamPosition - position);

    vec3 shadowDbg;

    vec4 lightSpacePos = (uShadowMap.lightTransform * vec4(position, 1.0));
    vec3 lightCoord = lightSpacePos.xyz / lightSpacePos.w;
    lightCoord = lightCoord + vec3(1.0);
    lightCoord *= 0.5;

    float shadow = calcShadow(position, uShadowMap, normal, lightDir, shadowDbg,
                              lightCoord);
    vec3 dirIllumination =
        calcIrradiance(normal, viewDir, albedo, vec3(5, 4, 4));
    vec3 ambientIllumination = vec3(0.03) * albedo;
    vec3 indirIllumination =
        doReflectiveShadowMapping(position, normal, lightCoord) * albedo *
        PI_INV;
    float ssao = calcSSAO(position, normal);

    color = vec4(dirIllumination * shadow + indirIllumination +
                     ambientIllumination * ssao,
                 1.0);
    // color = vec4(vec3(ssao), 1.0);
}
#include "DeferredRender.hpp"

#include <functional>
#include <random>

#include "glError.hpp"
using namespace std;
static void createRandomData(glm::vec3* randomData, int size) {
    std::default_random_engine eng;
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    eng.seed(std::time(0));
    for (int i = 0; i < size; ++i) {
        float r1 = dist(eng);
        float r2 = dist(eng);
        randomData[i].x = r1 * std::sin(2.0 * M_PI * r2);
        randomData[i].y = r1 * std::cos(2.0 * M_PI * r2);
        randomData[i].z = r1 * r1;
    }
}
#define RAND_SIZE 32

static void createRandomSampleRotation(vector<glm::vec3>& dest) {
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
    std::default_random_engine generator;
    std::vector<glm::vec3> ssaoNoise;
    for (GLuint i = 0; i < dest.size(); i++) {
        dest[i] = glm::vec3(randomFloats(generator) * 2.0 - 1.0,
                            randomFloats(generator) * 2.0 - 1.0, 0.0f);
    }
}

static void createHemisphereSamples(vector<glm::vec3>& dest) {
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
    std::default_random_engine generator;
    auto lerp = [](GLfloat a, GLfloat b, GLfloat f) {
        return a + f * (b - a);
    };
    for (GLuint i = 0; i < 64; ++i) {
        glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0,
                         randomFloats(generator) * 2.0 - 1.0,
                         randomFloats(generator));
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        GLfloat scale = GLfloat(i) / 64.0;

        // Scale samples s.t. they're more aligned to center of kernel
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        dest[i] = (sample);
    }
}

void DeferredRender::init() {
    m_framebuffer.init();

    m_texture.init();
    m_texture.setup(m_width, m_height, GL_RGB16F, GL_RGB, GL_FLOAT, 0);
    m_texture.setSizeFilter(GL_NEAREST, GL_NEAREST);

    m_framebuffer.attachTexture(m_texture, GL_COLOR_ATTACHMENT0, 0);

    m_random_tex.init();

    glm::vec3 randomData[RAND_SIZE];
    createRandomData(randomData, RAND_SIZE);
    m_random_tex.setup((unsigned char*)randomData, RAND_SIZE, 1, GL_RGB32F,
                       GL_RGB, GL_FLOAT, 0);
    m_random_tex.setSizeFilter(GL_LINEAR, GL_LINEAR);
    m_random_tex.setWrapFilter(GL_REPEAT);

    m_random_hemisphere_tex.init();

    m_ssao_sample_rotation.resize(16);
    createRandomSampleRotation(m_ssao_sample_rotation);
    m_random_hemisphere_tex.setup((unsigned char*)m_ssao_sample_rotation.data(),
                                  4, 4, GL_RGB16F, GL_RGB, GL_FLOAT, 0);
    m_random_hemisphere_tex.setSizeFilter(GL_NEAREST, GL_NEAREST);
    m_random_hemisphere_tex.setWrapFilter(GL_REPEAT);

    m_ssao_sample_kernel.resize(64);
    createHemisphereSamples(m_ssao_sample_kernel);
    glCheckError(__FILE__, __LINE__);
}

void DeferredRender::render(const Quad& quad, const GBuffer& gbuffer,
                            const RSMBuffer& rsmbuffer,
                            const ShadowMap& shadowmap, const Camera& cam,
                            glm::vec3 sunPosition) {
    m_framebuffer.bind();
    glClearColor(0, 0, 0, 1);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    m_shader.use();

    // gbuffer
    m_shader.setTexture("uGbuffer.positionTex", 0, gbuffer.getPosition());
    m_shader.setTexture("uGbuffer.depthTex", 1, gbuffer.getDepth());
    m_shader.setTexture("uGbuffer.normalTex", 2, gbuffer.getNormal());
    m_shader.setTexture("uGbuffer.albedoTex", 3, gbuffer.getAlbedo());

    // shadowmap
    m_shader.setTexture("uShadowMap.depthMap", 4, shadowmap.getDepthTexture());
    m_shader.setUniform("uShadowMap.lightTransform",
                        getLightSpaceTransform(sunPosition));

    // rsm buffer
    m_shader.setTexture("uRSMBuffer.positionTex", 5, rsmbuffer.getPosition());
    m_shader.setTexture("uRSMBuffer.depthTex", 6, rsmbuffer.getDepth());
    m_shader.setTexture("uRSMBuffer.normalTex", 7, rsmbuffer.getNormal());
    m_shader.setTexture("uRSMBuffer.fluxTex", 8, rsmbuffer.getFlux());

    m_shader.setTexture("uRSMRandomTex", 9, m_random_tex);
    m_shader.setUniform("uRSMRandomTexSize", RAND_SIZE);

    m_shader.setUniform("uLightDir", -sunPosition);
    m_shader.setUniform("uCamPosition", cam.getPosition());

    // ssao
    m_shader.setTexture("uRandomHemisphereTex", 10, m_random_hemisphere_tex);
    m_shader.setUniform("uView", cam.getViewMatrix());
    m_shader.setUniform("uProjection", cam.getProjectionMatrix());
    for (int i = 0; i < m_ssao_sample_kernel.size(); i++)
        m_shader.setUniform("uRandomSamples[" + std::to_string(i) + ']',
                            m_ssao_sample_kernel[i]);

    quad.draw();
    glCheckError(__FILE__, __LINE__);

    m_framebuffer.unbind();
}

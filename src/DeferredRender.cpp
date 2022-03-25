#include "DeferredRender.hpp"

#include <random>

#include "glError.hpp"
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
#define RAND_SIZE 100
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

    quad.draw();
    glCheckError(__FILE__, __LINE__);

    m_framebuffer.unbind();
}

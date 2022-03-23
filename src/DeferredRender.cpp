#include "DeferredRender.hpp"

#include "glError.hpp"
void DeferredRender::init() {
    m_framebuffer.init();

    m_texture.init();
    m_texture.setup(m_width, m_height, GL_RGB16F, GL_RGB, GL_FLOAT, 0);
    m_texture.setSizeFilter(GL_NEAREST, GL_NEAREST);
    m_framebuffer.attachTexture(m_texture, GL_COLOR_ATTACHMENT0, 0);

    glCheckError(__FILE__, __LINE__);
}

void DeferredRender::render(const Quad& quad, const GBuffer& gbuffer,
                            const Camera& cam, glm::vec3 sunPosition) {
    m_framebuffer.bind();
    glClearColor(0, 0, 0, 1);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    m_shader.use();

    m_shader.setTexture("uGbuffer.positionTex", 0, gbuffer.getPosition());
    m_shader.setTexture("uGbuffer.depthTex", 1, gbuffer.getDepth());
    m_shader.setTexture("uGbuffer.normalTex", 2, gbuffer.getNormal());
    m_shader.setTexture("uGbuffer.albedoTex", 3, gbuffer.getAlbedo());
    m_shader.setUniform("uLightDir", -sunPosition);
    m_shader.setUniform("uCamPosition", cam.getPosition());

    quad.draw();
    glCheckError(__FILE__, __LINE__);

    m_framebuffer.unbind();
}

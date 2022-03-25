#include "RSMBuffer.hpp"

#include "glError.hpp"
void RSMBuffer::init() {
    m_framebuffer.init();

    m_depth.init();
    m_depth.setup(m_width, m_height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT,
                  GL_FLOAT, 0);
    m_depth.setSizeFilter(GL_LINEAR, GL_LINEAR);
    m_framebuffer.attachTexture(m_depth, GL_DEPTH_ATTACHMENT, 0);

    m_position.init();
    m_position.setup(m_width, m_height, GL_RGB32F, GL_RGB, GL_FLOAT, 0);
    m_position.setSizeFilter(GL_LINEAR, GL_LINEAR);
    m_framebuffer.attachTexture(m_position, GL_COLOR_ATTACHMENT0, 0);

    m_normal.init();
    m_normal.setup(m_width, m_height, GL_RGB8_SNORM, GL_RGB, GL_FLOAT, 0);
    m_normal.setSizeFilter(GL_LINEAR, GL_LINEAR);
    m_framebuffer.attachTexture(m_normal, GL_COLOR_ATTACHMENT1, 0);

    m_flux.init();
    m_flux.setup(m_width, m_height, GL_RGB16F, GL_RGB, GL_FLOAT, 0);
    m_flux.setSizeFilter(GL_LINEAR, GL_LINEAR);
    GLfloat fluxBorder[4]{0, 0, 0, 0};
    m_flux.setClampToBorderFilter(fluxBorder);
    m_framebuffer.attachTexture(m_flux, GL_COLOR_ATTACHMENT2, 0);

    m_framebuffer.bind();
    GLenum attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                             GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    m_framebuffer.unbind();
    checkError();
}

void RSMBuffer::render(const Scene& scene, glm::vec3 light_pos) {
    m_framebuffer.bind();
    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);

    glViewport(0, 0, m_width, m_height);
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    m_shader.use();

    m_shader.setUniform("uModel", scene.getModelMatrix());
    m_shader.setUniform("uLightTransform", getLightSpaceTransform(light_pos));
    m_shader.setUniform("uLightDir", glm::normalize(-light_pos));
    m_shader.setUniform("uNormalTransform",
                        glm::transpose(glm::inverse(scene.getModelMatrix())));
    scene.draw(m_shader);
    checkError();

    m_framebuffer.unbind();
    glViewport(0, 0, m_viewport[2], m_viewport[3]);
}

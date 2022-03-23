#include "GBuffer.hpp"

#include <glError.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
using namespace std;

void GBuffer::init() {
    m_framebuffer.init();

    m_depth.init();
    m_depth.setup(m_width, m_height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT,
                  GL_FLOAT, 0);
    m_depth.setSizeFilter(GL_NEAREST, GL_NEAREST);
    m_framebuffer.attachTexture(m_depth, GL_DEPTH_ATTACHMENT, 0);

    m_position.init();
    m_position.setup(m_width, m_height, GL_RGB16F, GL_RGB, GL_FLOAT, 0);
    m_position.setSizeFilter(GL_NEAREST, GL_NEAREST);
    m_framebuffer.attachTexture(m_position, GL_COLOR_ATTACHMENT0, 0);

    m_normal.init();
    m_normal.setup(m_width, m_height, GL_RGB8_SNORM, GL_RGB, GL_FLOAT, 0);
    m_normal.setSizeFilter(GL_NEAREST, GL_NEAREST);
    m_framebuffer.attachTexture(m_normal, GL_COLOR_ATTACHMENT1, 0);

    m_albedo.init();
    m_albedo.setup(m_width, m_height, GL_RGBA, GL_RGBA, GL_FLOAT, 0);
    m_albedo.setSizeFilter(GL_NEAREST, GL_NEAREST);
    m_framebuffer.attachTexture(m_albedo, GL_COLOR_ATTACHMENT2, 0);

    m_framebuffer.bind();
    GLenum attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                             GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    m_framebuffer.unbind();
    glCheckError(__FILE__, __LINE__);
}

void GBuffer::render(const Scene& scene, const Camera& cam) {
    m_framebuffer.bind();
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    m_shader.use();

    m_shader.setUniform("model", scene.getModelMatrix());
    m_shader.setUniform("view", cam.getViewMatrix());
    m_shader.setUniform("normalTransform",
                        glm::transpose(glm::inverse(scene.getModelMatrix())));
    m_shader.setUniform("projection", cam.getProjectionMatrix());
    glCheckError(__FILE__, __LINE__);
    scene.draw(m_shader);
    glCheckError(__FILE__, __LINE__);

    m_framebuffer.unbind();
}

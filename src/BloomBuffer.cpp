#include "BloomBuffer.hpp"

#include <glad/glad.h>

#include "glError.hpp"
void BloomBuffer::init() {
    m_texture.init();
    m_texture.setup(m_width, m_height, GL_RGB16F, GL_RGB, GL_FLOAT, 0);
    m_texture.setSizeFilter(GL_LINEAR, GL_LINEAR);
    m_texture.setWrapFilter(GL_REPEAT);

    m_framebuffer.init();
    m_framebuffer.attachTexture(m_texture, GL_COLOR_ATTACHMENT0, 0);

    m_blurer.init(GL_RGB16F, GL_RGB, GL_FLOAT);
}

void BloomBuffer::render(const Quad& quad, const Texture& input) {
    m_framebuffer.bind();
    checkError();

    glViewport(0, 0, m_width, m_height);
    glDisable(GL_DEPTH_TEST);

    m_shader.use();
    m_shader.setTexture("uInputTexture", 0, input);
    quad.draw();

    m_framebuffer.unbind();

    for (int i = 0; i < 3; i++) {
        m_blurer.blur(m_texture, 9);
    }
}

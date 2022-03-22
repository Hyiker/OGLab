#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <glad/glad.h>
class Framebuffer {
    GLuint m_fbo{};

   public:
    Framebuffer() = default;
    void init() {
        glGenFramebuffers(1, &m_fbo);
    }
    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    }
};

#endif /* FRAMEBUFFER_H */

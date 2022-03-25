#ifndef GAUSSIANBLUR_H
#define GAUSSIANBLUR_H
#include <glad/glad.h>

#include "Framebuffer.hpp"
#include "Quad.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
class GaussianBlur {
    Quad m_quad;
    ShaderProgram m_shader;
    Framebuffer m_fbs[2];
    Texture m_texs[2];
    GLsizei m_width, m_height;

   public:
    GaussianBlur(GLsizei width, GLsizei height);
    void init(GLenum internalformat, GLenum format, GLenum type);
    void blur(Texture& tex);
};

#endif /* GAUSSIANBLUR_H */

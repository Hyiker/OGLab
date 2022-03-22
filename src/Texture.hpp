#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>

#include <string>
class Texture {
    GLuint m_id;

   public:
    void init() { glGenTextures(1, &m_id); }
    void bind() const { glBindTexture(GL_TEXTURE_2D, m_id); }
    void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
    GLuint getId() const { return m_id; };
    void generateMipmap() {
        bind();
        glGenerateMipmap(GL_TEXTURE_2D);
        unbind();
    }
    void setSizeFilter(GLenum min_filter, GLenum mag_filter) {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
        unbind();
    }
    void setWrapFilter(GLenum filter) {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, filter);
        unbind();
    }
    void setup(GLsizei width, GLsizei height, GLenum internalformat,
               GLenum type, GLsizei levels) {
        bind();
        glTexImage2D(GL_TEXTURE_2D, levels, internalformat, width, height, 0,
                     GL_RGB, type, nullptr);
        unbind();
    }
    void setup(unsigned char* data, GLsizei width, GLsizei height,
               GLenum format) {
        glGenTextures(1, &m_id);

        glBindTexture(GL_TEXTURE_2D, this->m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
};
#endif /* TEXTURE_H */

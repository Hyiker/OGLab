#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>

#include <string>
class Texture {
    GLuint m_id;

   public:
    void init() {
        glGenTextures(1, &m_id);
    }
    GLuint getId() {
        return m_id;
    };
    void setup(unsigned char* data, int width, int height, GLenum format) {
        glGenTextures(1, &m_id);

        glBindTexture(GL_TEXTURE_2D, this->m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 8);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
};
#endif /* TEXTURE_H */

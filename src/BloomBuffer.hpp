#ifndef BLOOMBUFFER_H
#define BLOOMBUFFER_H
#include "Framebuffer.hpp"
#include "GaussianBlur.hpp"
#include "Quad.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
class BloomBuffer {
    ShaderProgram m_shader;
    Framebuffer m_framebuffer;
    Texture m_texture;
    int m_width, m_height;
    GaussianBlur m_blurer;

   public:
    BloomBuffer(ShaderProgram&& shaderProgram, int width = 2048,
                int height = 2048)
        : m_shader{std::move(shaderProgram)},
          m_width{width},
          m_height{height},
          m_blurer{width, height} {}

    void init();
    void render(const Quad& quad, const Texture& input);
    const Texture& getBloomTexture() const { return m_texture; }
};
#endif /* BLOOMBUFFER_H */

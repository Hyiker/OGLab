#ifndef GBUFFER_H
#define GBUFFER_H
#include "Camera.hpp"
#include "Framebuffer.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
class GBuffer {
    Texture m_position, m_depth, m_normal, m_albedo;
    Framebuffer m_framebuffer;
    ShaderProgram m_shader;
    int m_width, m_height;

   public:
    GBuffer(ShaderProgram&& shaderProgram, int width = 1080, int height = 1080)
        : m_shader{std::move(shaderProgram)},
          m_width{width},
          m_height{height} {}
    void init();
    void render(const Scene& scene, const Camera& cam);
    const Texture& getPosition() const { return m_position; }
    const Texture& getNormal() const { return m_normal; }
    const Texture& getAlbedo() const { return m_albedo; }
};

#endif /* GBUFFER_H */

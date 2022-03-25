#ifndef RSMBUFFER_H
#define RSMBUFFER_H
#include <glm/vec3.hpp>

#include "Framebuffer.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
class RSMBuffer {
    Texture m_depth, m_position, m_normal, m_flux;
    Framebuffer m_framebuffer;
    ShaderProgram m_shader;
    int m_width, m_height;

   public:
    RSMBuffer(ShaderProgram&& shaderProgram, int width = 2048,
              int height = 2048)
        : m_shader{std::move(shaderProgram)},
          m_width{width},
          m_height{height} {}
    void init();
    void render(const Scene& scene, glm::vec3 light_pos);
    const Texture& getDepth() const { return m_depth; }
    const Texture& getPosition() const { return m_position; }
    const Texture& getNormal() const { return m_normal; }
    const Texture& getFlux() const { return m_flux; }
};
#endif /* RSMBUFFER_H */

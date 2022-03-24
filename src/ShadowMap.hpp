#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include <glm/vec3.hpp>

#include "Framebuffer.hpp"
#include "Scene.hpp"
#include "Texture.hpp"
class ShadowMap {
    Texture m_depth;
    Framebuffer m_framebuffer;
    ShaderProgram m_shader;
    int m_width, m_height;

   public:
    ShadowMap(ShaderProgram&& shaderProgram, int width = 1080,
              int height = 1080)
        : m_shader{std::move(shaderProgram)},
          m_width{width},
          m_height{height} {}
    void init();
    void render(const Scene& scene, glm::vec3 sunPosition);
    glm::mat4 getLightSpaceTransform(glm::vec3 lightPosition) const;
    const Texture& getDepthTexture() const { return m_depth; }
};
#endif /* SHADOWMAP_H */

#ifndef DEFERREDRENDER_H
#define DEFERREDRENDER_H
#include "Camera.hpp"
#include "Framebuffer.hpp"
#include "GBuffer.hpp"
#include "Quad.hpp"
#include "RSMBuffer.hpp"
#include "Shader.hpp"
#include "ShadowMap.hpp"
#include "Texture.hpp"
class DeferredRender {
    Texture m_texture;

    Framebuffer m_framebuffer;
    ShaderProgram m_shader;
    Texture m_random_tex, m_random_hemisphere_tex;
    std::vector<glm::vec3> m_ssao_sample_kernel, m_ssao_sample_rotation;
    int m_width, m_height;

   public:
    DeferredRender(ShaderProgram&& shaderProgram, int width = 1080,
                   int height = 1080)
        : m_shader{std::move(shaderProgram)},
          m_width{width},
          m_height{height} {}
    void init();
    void render(const Quad& quad, const GBuffer& gbuffer,
                const RSMBuffer& rsmbuffer, const ShadowMap& shadowmap,
                const Camera& cam, glm::vec3 sunPosition);
    const Texture& getTexture() const { return m_texture; }
};

#endif /* DEFERREDRENDER_H */

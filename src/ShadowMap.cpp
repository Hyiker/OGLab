#include "ShadowMap.hpp"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

#include "glError.hpp"
using namespace glm;
void ShadowMap::init() {
    m_framebuffer.init();
    m_framebuffer.bind();
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    m_framebuffer.unbind();

    m_depth.init();
    m_depth.setup(m_width, m_height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT,
                  GL_FLOAT, 0);
    m_depth.setSizeFilter(GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST);
    GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
    m_depth.setClampToBorderFilter(borderColor);
    m_framebuffer.attachTexture(m_depth, GL_DEPTH_ATTACHMENT, 0);
}

void ShadowMap::render(const Scene& scene, glm::vec3 sunPosition) {
    m_framebuffer.bind();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    m_shader.use();
    m_shader.setUniform("uModel", scene.getModelMatrix());
    m_shader.setUniform("uLightTransform", getLightSpaceTransform(sunPosition));
    m_depth.generateMipmap();
    scene.draw(m_shader);
    checkError();
    glCullFace(GL_BACK);

    m_framebuffer.unbind();
}

glm::mat4 ShadowMap::getLightSpaceTransform(glm::vec3 lightPosition) const {
    vec3 target(0, 0, 0);
    vec3 dir = normalize(target - lightPosition);
    vec3 up{0.0f, 1.0f, 0.0f};
    if (dir.x == 0.0f && dir.z == 0.0f) up = glm::vec3(1.0f, 0.0f, 0.0f);
    return ortho(-1.5f, 1.5f, -1.5f, 1.5f, -1.5f, 1.5f) *
           lookAt(vec3(0.0f), dir, up);
}

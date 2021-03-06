/**
 * MyApplication.hpp skeleton
 * Contributors:
 *      * Arthur Sonzogni
 * Licence:
 *      * MIT
 */

#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <Scene.hpp>
#include <string>
#include <vector>

#include "Application.hpp"
#include "BloomBuffer.hpp"
#include "Camera.hpp"
#include "DeferredRender.hpp"
#include "Framebuffer.hpp"
#include "GBuffer.hpp"
#include "Quad.hpp"
#include "RSMBuffer.hpp"
#include "Shader.hpp"
#include "ShadowMap.hpp"

class MyApplication : public Application {
    Scene m_scene;
    ShaderProgram m_finalsp;
    glm::vec3 m_sun_position;
    bool m_sun_moved{true};

    ShadowMap m_shadowmap;
    RSMBuffer m_rsmbuffer;
    GBuffer m_gbuffer;
    DeferredRender m_defrender;
    BloomBuffer m_bloombuffer;

    Quad m_quad;
    Camera m_cam;

   public:
    MyApplication(const std::string& path, int width = 640, int height = 480);
    Camera& getCamera() { return m_cam; }

   protected:
    void gui();
    virtual void loop();

   private:
    void cameraMove();
    void sunMove();
};

#endif /* MYAPPLICATION_H */

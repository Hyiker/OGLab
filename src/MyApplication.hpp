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
#include "Camera.hpp"
#include "Framebuffer.hpp"
#include "GBuffer.hpp"
#include "Quad.hpp"
#include "Shader.hpp"

class MyApplication : public Application {
    Scene m_scene;
    ShaderProgram m_finalsp;
    glm::vec3 m_sun_position;

    GBuffer m_gbuffer;

    Quad m_screenquad;
    Camera m_cam;

   public:
    MyApplication(const std::string& path, int width = 640, int height = 480);
    Camera& getCamera() { return m_cam; }

   protected:
    void gui();
    virtual void loop();

   private:
    void processInput();
};

#endif /* MYAPPLICATION_H */

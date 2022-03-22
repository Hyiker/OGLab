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

#include "Application.hpp"
#include "Camera.hpp"
#include "Framebuffer.hpp"
#include "Quad.hpp"
#include "Shader.hpp"

class MyApplication : public Application {
    Scene m_scene;
    Shader vs;
    Shader fs;
    ShaderProgram sp;
    Shader m_finalvs, m_finalfs;
    ShaderProgram m_finalsp;
    glm::vec3 m_sun_position;
    Texture m_fb1_tex;
    Framebuffer m_fb1;
    Renderbuffer m_rb1;

    Quad m_screenquad;

   public:
    MyApplication(const std::string& path, int width = 640, int height = 480);
    Camera cam;

   protected:
    void gui();
    virtual void loop();

   private:
    void processInput();
};

#endif /* MYAPPLICATION_H */

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
#include "Shader.hpp"

class MyApplication : public Application {
    Scene m_scene;
    Shader vs;
    Shader fs;
    ShaderProgram sp;
    glm::vec3 m_sun_position;

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

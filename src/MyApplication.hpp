/**
 * MyApplication.hpp skeleton
 * Contributors:
 *      * Arthur Sonzogni
 * Licence:
 *      * MIT
 */

#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include "Application.hpp"
#include "Shader.hpp"

class MyApplication : public Application {
 public:
  MyApplication();

 protected:
  virtual void loop();

 private:
  const int size = 100;

  // shader
  Shader vertexShader;
  Shader fragmentShader;
  ShaderProgram shaderProgram;

  // shader matrix uniform
  glm::mat4 projection = glm::mat4(1.0);
  glm::mat4 view = glm::mat4(1.0);

  // VBO/VAO/ibo
  GLuint vao, vbo, ibo;
};

#endif /* MYAPPLICATION_H */

/**
 * MyApplication.cpp
 * Contributors:
 *      * Arthur Sonzogni (author)
 * Licence:
 *      * MIT
 */
#include "MyApplication.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/matrix.hpp>
#include <iostream>
#include <vector>

#include "asset.hpp"
#include "glError.hpp"

struct VertexType {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec4 color;
};

MyApplication::MyApplication() : Application() {}

void MyApplication::loop() {
  if (glfwWindowShouldClose(getWindow())) {
    exit();
  }
}

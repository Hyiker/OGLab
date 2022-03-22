/**
 * MyApplication.cpp
 * Contributors:
 *      * Arthur Sonzogni (author)
 * Licence:
 *      * MIT
 */
#include "MyApplication.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/matrix.hpp>
#include <iostream>
#include <vector>

#include "asset.hpp"
#include "glError.hpp"

using namespace std;
using namespace glm;

static void mouseCallback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    MyApplication* myapp =
        static_cast<MyApplication*>(glfwGetWindowUserPointer(window));
    static bool firstMouse = true;
    static float lastX = myapp->getWidth() / 2.0;
    static float lastY = myapp->getHeight() / 2.0;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;
    myapp->cam.processMouseMovement(xoffset, yoffset);
}

MyApplication::MyApplication(const string& path, int width, int height)
    : Application(width, height),
      scene(path),
      vs(SHADER_DIR "/shader.vert", GL_VERTEX_SHADER),
      fs(SHADER_DIR "/shader.frag", GL_FRAGMENT_SHADER),
      sp({vs, fs}),
      cam(vec3(-45, 38, -3), glm::vec3(0.0f, 1.0f, 0.0f), -2.2, -25) {
    scene.scale(vec3(0.05));
    glEnable(GL_DEPTH_TEST);

    glfwSetWindowUserPointer(getWindow(), this);
    glfwSetCursorPosCallback(getWindow(), mouseCallback);

    glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void MyApplication::processInput() {
    if (glfwGetKey(getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        cam.processKeyboard(CameraMovement::FORWARD, getFrameDeltaTime());
    if (glfwGetKey(getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        cam.processKeyboard(CameraMovement::BACKWARD, getFrameDeltaTime());
    if (glfwGetKey(getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        cam.processKeyboard(CameraMovement::LEFT, getFrameDeltaTime());
    if (glfwGetKey(getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        cam.processKeyboard(CameraMovement::RIGHT, getFrameDeltaTime());
    if (glfwGetKey(getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) exit();
}
void MyApplication::loop() {
    if (glfwWindowShouldClose(getWindow())) {
        exit();
    }
    // ImGui
    {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(float(getWidth()) / 3.0, getHeight()));
        ImGui::Begin("Debug Panel");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);

        // get version info
        const GLubyte* renderer = glGetString(GL_RENDERER);
        const GLubyte* version = glGetString(GL_VERSION);
        ImGui::Text("Renderer: %s", renderer);
        ImGui::Text("OpenGL Version: %s", version);

        ImGui::Separator();
        ImGui::Text("Meshes: %lu, Vertices: %lu", scene.countMesh(),
                    scene.countVertex());
        ImGui::Text("Camera Position: (%.1f, %.1f, %.1f)", cam.position.x,
                    cam.position.y, cam.position.z);
        ImGui::Text("Camera Pitch: %.1f, Yaw: %.1f", cam.pitch, cam.yaw);

        ImGui::End();
    }
    processInput();
    glClearColor(0, 0, 0, 1);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    sp.use();
    sp.setUniform("model", scene.getModelMatrix());
    sp.setUniform("view", cam.getViewMatrix());
    sp.setUniform("normalTransform",
                  glm::transpose(glm::inverse(scene.getModelMatrix())));
    sp.setUniform("projection",
                  perspective(45.f, float(getWidth()) / float(getHeight()),
                              0.01f, 500.0f));
    scene.draw(sp);
    glCheckError(__FILE__, __LINE__);

    glBindVertexArray(0);
}

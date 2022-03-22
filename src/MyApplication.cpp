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

#include "Framebuffer.hpp"
#include "asset.hpp"
#include "glError.hpp"

using namespace std;
using namespace glm;

static void mouseCallback(GLFWwindow* window, double xposIn, double yposIn) {
    static bool firstMouse = true;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        firstMouse = true;
        return;
    }
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    MyApplication* myapp =
        static_cast<MyApplication*>(glfwGetWindowUserPointer(window));
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
      m_scene(path),
      vs(SHADER_DIR "/shader.vert", GL_VERTEX_SHADER),
      fs(SHADER_DIR "/shader.frag", GL_FRAGMENT_SHADER),
      sp({vs, fs}),
      m_finalvs(SHADER_DIR "/finalShader.vert", GL_VERTEX_SHADER),
      m_finalfs(SHADER_DIR "/finalShader.frag", GL_FRAGMENT_SHADER),
      m_finalsp({m_finalvs, m_finalfs}),
      m_sun_position(0, 35, -2),
      cam(vec3(-34, 92, -2), glm::vec3(0.0f, 1.0f, 0.0f), -2.2, -25) {
    m_scene.scale(vec3(0.1));
    glEnable(GL_DEPTH_TEST);
    m_rb1.init(GL_DEPTH_ATTACHMENT, getWidth(), getHeight());
    m_fb1_tex.init();
    m_fb1_tex.setup(getWidth(), getHeight(), GL_RGB16, GL_FLOAT, 0);

    m_fb1.init();
    m_fb1.attachTexture(m_fb1_tex, GL_COLOR_ATTACHMENT0, 0);
    m_fb1.attachRenderbuffer(m_rb1, GL_DEPTH_ATTACHMENT);

    glfwSetWindowUserPointer(getWindow(), this);
    glfwSetCursorPosCallback(getWindow(), mouseCallback);
}
void MyApplication::gui() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(float(getWidth()) / 3.0, getHeight()));
    ImGui::Begin("Debug Panel");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    ImGui::Text("Renderer: %s", renderer);
    ImGui::Text("OpenGL Version: %s", version);

    ImGui::Separator();
    ImGui::Text("Meshes: %lu, Vertices: %lu", m_scene.countMesh(),
                m_scene.countVertex());
    ImGui::Text("Camera Position: (%.1f, %.1f, %.1f)", cam.position.x,
                cam.position.y, cam.position.z);
    ImGui::Text("Camera Pitch: %.1f, Yaw: %.1f", cam.pitch, cam.yaw);

    ImGui::End();
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
    processInput();

    glClearColor(0.53, 0.81, 0.92, 1);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    sp.use();
    sp.setUniform("model", m_scene.getModelMatrix());
    sp.setUniform("view", cam.getViewMatrix());
    sp.setUniform("normalTransform",
                  glm::transpose(glm::inverse(m_scene.getModelMatrix())));
    sp.setUniform("projection",
                  perspective(45.f, float(getWidth()) / float(getHeight()),
                              0.01f, 500.0f));
    sp.setUniform("sunPosition", m_sun_position);
    m_scene.draw(sp);
    glCheckError(__FILE__, __LINE__);
    gui();

    glBindVertexArray(0);
}

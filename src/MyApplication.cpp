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
    static float lastX = myapp->getWindowWidth() / 2.0;
    static float lastY = myapp->getWindowHeight() / 2.0;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;
    myapp->getCamera().processMouseMovement(xoffset, yoffset);
}

MyApplication::MyApplication(const string& path, int width, int height)
    : Application(width, height),
      m_scene(path),
      m_finalsp({{SHADER_DIR "/finalShader.vert", GL_VERTEX_SHADER},
                 {SHADER_DIR "/finalShader.frag", GL_FRAGMENT_SHADER}}),
      m_sun_position(-53, 159, 2),
      m_gbuffer{
          ShaderProgram({{SHADER_DIR "/shader.vert", GL_VERTEX_SHADER},
                         {SHADER_DIR "/gShader.frag", GL_FRAGMENT_SHADER}}),
          getFramebufferWidth(), getFramebufferHeight()},
      m_cam(vec3(21, 74, -2), glm::vec3(0.0f, 1.0f, 0.0f), -2.2, -25) {
    m_scene.scale(vec3(0.1));

    glEnable(GL_DEPTH_TEST);
    glCheckError(__FILE__, __LINE__);
    m_gbuffer.init();
    glCheckError(__FILE__, __LINE__);

    glfwSetWindowUserPointer(getWindow(), this);
    glfwSetCursorPosCallback(getWindow(), mouseCallback);
}
void MyApplication::gui() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(
        ImVec2(float(getWindowWidth()) / 3.0, getWindowHeight()));
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
    ImGui::Text("Camera Position: (%.1f, %.1f, %.1f)", m_cam.position.x,
                m_cam.position.y, m_cam.position.z);
    ImGui::Text("Camera Pitch: %.1f, Yaw: %.1f", m_cam.pitch, m_cam.yaw);

    ImGui::End();
}
void MyApplication::processInput() {
    if (glfwGetKey(getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        m_cam.processKeyboard(CameraMovement::FORWARD, getFrameDeltaTime());
    if (glfwGetKey(getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        m_cam.processKeyboard(CameraMovement::BACKWARD, getFrameDeltaTime());
    if (glfwGetKey(getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        m_cam.processKeyboard(CameraMovement::LEFT, getFrameDeltaTime());
    if (glfwGetKey(getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        m_cam.processKeyboard(CameraMovement::RIGHT, getFrameDeltaTime());
    if (glfwGetKey(getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) exit();
}
void MyApplication::loop() {
    if (glfwWindowShouldClose(getWindow())) {
        exit();
    }
    processInput();
    glClearColor(0.53, 0.81, 0.92, 1);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    m_gbuffer.render(m_scene, m_cam);

    glViewport(0, 0, getFramebufferWidth(), getFramebufferHeight());
    glCheckError(__FILE__, __LINE__);

    glClearColor(0, 0, 0, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    m_finalsp.use();
    m_finalsp.setTexture("screenTexture", 0, m_gbuffer.getNormal(),
                         GL_TEXTURE_2D);
    m_screenquad.draw();

    glCheckError(__FILE__, __LINE__);
    gui();

    glBindVertexArray(0);
}

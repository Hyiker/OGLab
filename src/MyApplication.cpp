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
      m_sun_position(0, 10.0, 0),
      m_shadowmap{
          ShaderProgram(
              {{SHADER_DIR "/shadowmapShader.vert", GL_VERTEX_SHADER},
               {SHADER_DIR "/shadowmapShader.frag", GL_FRAGMENT_SHADER}}),
          2048, 2048},
      m_rsmbuffer{
          ShaderProgram({{SHADER_DIR "/rsmShader.vert", GL_VERTEX_SHADER},
                         {SHADER_DIR "/rsmShader.frag", GL_FRAGMENT_SHADER}}),
          2048, 2048},
      m_gbuffer{
          ShaderProgram({{SHADER_DIR "/shader.vert", GL_VERTEX_SHADER},
                         {SHADER_DIR "/gShader.frag", GL_FRAGMENT_SHADER}}),
          getFramebufferWidth(), getFramebufferHeight()},
      m_defrender{
          ShaderProgram(
              {{SHADER_DIR "/deferredShader.vert", GL_VERTEX_SHADER},
               {SHADER_DIR "/deferredShader.frag", GL_FRAGMENT_SHADER}}),
          getFramebufferWidth(), getFramebufferHeight()},
      m_cam(vec3(-0.65, 0.4, 0), glm::vec3(0.0f, 1.0f, 0.0f), -11.5, -9.2) {
    m_scene.scale(vec3(0.0005));

    glEnable(GL_DEPTH_TEST);
    checkError();
    m_gbuffer.init();
    checkError();
    m_defrender.init();
    checkError();
    m_shadowmap.init();
    checkError();
    m_rsmbuffer.init();

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

    // information
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    ImGui::Text("Renderer: %s", renderer);
    ImGui::Text("OpenGL Version: %s", version);

    // object information
    ImGui::Separator();
    ImGui::Text("Meshes: %lu, Vertices: %lu", m_scene.countMesh(),
                m_scene.countVertex());
    ImGui::Text("Camera Position: (%.1f, %.1f, %.1f)", m_cam.position.x,
                m_cam.position.y, m_cam.position.z);
    ImGui::Text("Camera Pitch: %.1f, Yaw: %.1f", m_cam.pitch, m_cam.yaw);
    ImGui::Text("Sun Position: (%.1f, %.1f, %.1f)", m_sun_position.x,
                m_sun_position.y, m_sun_position.z);

    ImGui::End();
}
void MyApplication::cameraMove() {
    if (glfwGetKey(getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        m_cam.processKeyboard(CameraMovement::FORWARD, getFrameDeltaTime());
    if (glfwGetKey(getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        m_cam.processKeyboard(CameraMovement::BACKWARD, getFrameDeltaTime());
    if (glfwGetKey(getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        m_cam.processKeyboard(CameraMovement::LEFT, getFrameDeltaTime());
    if (glfwGetKey(getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        m_cam.processKeyboard(CameraMovement::RIGHT, getFrameDeltaTime());
}

void MyApplication::sunMove() {
    if (glfwGetKey(getWindow(), GLFW_KEY_UP) == GLFW_PRESS) {
        m_sun_position.x -= 10.0 * getFrameDeltaTime();
        m_sun_moved = true;
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
        m_sun_position.x += 10.0 * getFrameDeltaTime();
        m_sun_moved = true;
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS) {
        m_sun_position.z += 10.0 * getFrameDeltaTime();
        m_sun_moved = true;
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
        m_sun_position.z -= 10.0 * getFrameDeltaTime();
        m_sun_moved = true;
    }
}
void MyApplication::loop() {
    if (glfwWindowShouldClose(getWindow()) ||
        glfwGetKey(getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        exit();
    bool light_rerendered_flag = false;
    // input interact
    cameraMove();
    sunMove();

    // shadowmap
    if (m_sun_moved) {
        m_shadowmap.render(m_scene, m_sun_position);
        light_rerendered_flag = true;
    }
    checkError();

    // rsm buffer
    if (m_sun_moved) {
        m_rsmbuffer.render(m_scene, m_sun_position);
        light_rerendered_flag = true;
    }

    // render gbuffer
    m_gbuffer.render(m_scene, m_cam);
    checkError();

    // use gbuffer
    m_defrender.render(m_quad, m_gbuffer, m_rsmbuffer, m_shadowmap, m_cam,
                       m_sun_position);
    checkError();

    // screen quad
    glClearColor(0, 0, 0, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    m_finalsp.use();
    m_finalsp.setTexture("screenTexture", 0, m_defrender.getTexture());
    m_finalsp.setTexture("lightDepthTexture", 1, m_shadowmap.getDepthTexture());
    m_quad.draw();

    glCheckError(__FILE__, __LINE__);

    glBindVertexArray(0);
    gui();

    if (light_rerendered_flag && m_sun_moved) m_sun_moved = false;
}

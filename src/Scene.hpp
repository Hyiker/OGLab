#ifndef SCENE_H
#define SCENE_H
#include <glad/glad.h>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};
struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    GLuint vao, vbo, ebo;
    void prepare();

    void draw();
};
class Scene {
    std::vector<Mesh> m_meshes;
    glm::mat4 m_modelmat;

   public:
    void scale(glm::vec3 ratio);
    void translate(glm::vec3 pos);
    glm::mat4 getModelMatrix() const;
    void draw();
    Scene(const std::string& path);
};

#endif /* SCENE_H */

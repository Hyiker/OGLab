#include "Scene.hpp"

#include <glad/glad.h>

#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

using namespace std;
using namespace tinyobj;
namespace fs = std::filesystem;
void Mesh::prepare() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Mesh::draw() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()),
                   GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void Scene::scale(glm::vec3 ratio) {
    m_modelmat = glm::scale(m_modelmat, ratio);
}

void Scene::translate(glm::vec3 pos) {
    m_modelmat = glm::translate(m_modelmat, pos);
}

glm::mat4 Scene::getModelMatrix() const {
    return m_modelmat;
}

void Scene::draw() {
    for (auto& mesh : m_meshes) {
        mesh.draw();
    }
}

vector<Mesh> readObject(const std::string& path) {
    ObjReader reader;
    ObjReaderConfig config;

    if (!reader.ParseFromFile(path, config)) {
        if (!reader.Error().empty()) {
            throw runtime_error("TinyObjReader: " + reader.Error());
        }
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();
    vector<Mesh> meshes;

    for (auto& shape : shapes) {
        Mesh mesh;
        for (auto& idx : shape.mesh.indices) {
            Vertex vertex;
            // access to vertex

            vertex.position.x =
                attrib.vertices[3 * size_t(idx.vertex_index) + 0];
            vertex.position.y =
                attrib.vertices[3 * size_t(idx.vertex_index) + 1];
            vertex.position.z =
                attrib.vertices[3 * size_t(idx.vertex_index) + 2];

            // Check if `normal_index` is zero or positive. negative = no
            // normal data
            if (idx.normal_index >= 0) {
                vertex.normal.x =
                    attrib.normals[3 * size_t(idx.normal_index) + 0];
                vertex.normal.y =
                    attrib.normals[3 * size_t(idx.normal_index) + 1];
                vertex.normal.z =
                    attrib.normals[3 * size_t(idx.normal_index) + 2];
            }

            // Check if `texcoord_index` is zero or positive. negative = no
            // texcoord data
            if (idx.texcoord_index >= 0) {
                vertex.texCoord.x =
                    attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                vertex.texCoord.y =
                    attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
            }
            mesh.vertices.push_back(move(vertex));
            mesh.indices.push_back(mesh.indices.size());
        }
        meshes.push_back(mesh);
    }
    return meshes;
}
Scene::Scene(const std::string& path) {
    string abspath = fs::absolute(path);
    cout << "loading model from " << abspath << '\n';
    m_meshes = readObject(path);

    // prepare mesh vao ebo
    for (auto& mesh : m_meshes) {
        mesh.prepare();
    }
}

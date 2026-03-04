#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices);
    ~Mesh();
    void Draw();

private:
    std::vector<Vertex> m_Vertices;
    unsigned int VAO, VBO;
    void SetupMesh();
};

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 Position;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    unsigned int VAO;

    Mesh(std::vector<Vertex> vertices);
    void Draw();

private:
    unsigned int VBO;
    void SetupMesh();
};

#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices) {
    this->vertices = vertices;
    SetupMesh();
}

void Mesh::SetupMesh() {
    // Generate the IDs for our buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 1. Bind the Vertex Array Object first
    glBindVertexArray(VAO);

    // 2. Bind and set the Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // 3. Set the Vertex Attribute Pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Unbind everything so we don't accidentally modify it later
    glBindVertexArray(0);
}

void Mesh::Draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

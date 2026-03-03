#pragma once

#include "Transform.h"

#include <vector>
#include <string>

class Mesh;

class GameObject {
public:
    std::string name = "Object";
    Transform transform;
    bool markedForDeletion = false;

    GameObject* parent = nullptr;
    std::vector<GameObject*> children;

    glm::mat4 GetWorldMatrix() const;
    void AddChild(GameObject* child);

    Mesh* mesh = nullptr;
    glm::vec3 color = {1.0f, 0.5f, 0.2f};
};

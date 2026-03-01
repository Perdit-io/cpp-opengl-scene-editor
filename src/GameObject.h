#pragma once

#include "Transform.h"

#include <vector>
#include <string>

class GameObject {
public:
    std::string name = "Object";
    Transform transform;

    GameObject* parent = nullptr;
    std::vector<GameObject*> children;

    glm::mat4 GetWorldMatrix() const {
        if (parent) {
            return parent->GetWorldMatrix() * transform.GetLocalMatrix();
        }
        return transform.GetLocalMatrix();
    }

    void AddChild(GameObject* child) {
        child->parent = this;
        children.push_back(child);
    }
};

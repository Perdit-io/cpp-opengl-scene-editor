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

    void SetParent(GameObject* newParent);
    GameObject* GetParent() const { return m_Parent; }
    const std::vector<GameObject*>& GetChildren() const { return m_Children; }

    glm::mat4 GetWorldMatrix() const;

    Mesh* mesh = nullptr;
    glm::vec3 color = {1.0f, 0.5f, 0.2f};

    bool IsDescendantOf(GameObject* potentialAncestor) const;

private:
    GameObject* m_Parent = nullptr;
    std::vector<GameObject*> m_Children;
};

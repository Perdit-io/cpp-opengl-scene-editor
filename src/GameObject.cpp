#include "GameObject.h"

glm::mat4 GameObject::GetWorldMatrix() const {
    glm::mat4 model = transform.GetLocalMatrix();

    if (parent) {
        return parent->GetWorldMatrix() * model;
    }
    return model;
}

void GameObject::AddChild(GameObject* child) {
    child->parent = this;
    children.push_back(child);
}

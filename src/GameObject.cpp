#include "GameObject.h"

glm::mat4 GameObject::GetWorldMatrix() const {
    if (parent) {
        return parent->GetWorldMatrix() * transform.GetLocalMatrix();
    }
    return transform.GetLocalMatrix();
}

void GameObject::AddChild(GameObject* child) {
    child->parent = this;
    children.push_back(child);
}

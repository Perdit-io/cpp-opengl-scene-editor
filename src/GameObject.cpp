#include "GameObject.h"

glm::mat4 GameObject::GetWorldMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, transform.position);

    model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));

    model = glm::scale(model, transform.scale);

    if (parent) {
        return parent->GetWorldMatrix() * model;
    }
    return model;
}

void GameObject::AddChild(GameObject* child) {
    child->parent = this;
    children.push_back(child);
}

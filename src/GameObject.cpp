#include "GameObject.h"
#include <algorithm>

glm::mat4 GameObject::GetWorldMatrix() const {
    glm::mat4 model = transform.GetLocalMatrix();

    if (m_Parent) {
        return m_Parent->GetWorldMatrix() * model;
    }
    return model;
}

void GameObject::SetParent(GameObject* newParent) {
    if (m_Parent == newParent || newParent == this) return;

    glm::mat4 worldMatrix = GetWorldMatrix();

    if (m_Parent) {
        auto& siblings = m_Parent->m_Children;
        siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
    }

    m_Parent = newParent;

    if (m_Parent) {
        m_Parent->m_Children.push_back(this);
        transform.position.x = 0.0f;
        transform.position.y = 0.0f;
        transform.position.z = 0.0f;
        transform.rotation.x = 0.0f;
        transform.rotation.y = 0.0f;
        transform.rotation.z = 0.0f;
    }
}

bool GameObject::IsDescendantOf(GameObject* potentialAncestor) const {
    GameObject* current = GetParent();
    while (current != nullptr) {
        if (current == potentialAncestor) {
            return true;
        }
        current = current->GetParent();
    }
    return false;
}

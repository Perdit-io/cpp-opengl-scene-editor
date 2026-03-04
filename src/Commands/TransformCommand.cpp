#include "TransformCommand.h"
#include "GameObject.h"

TransformCommand::TransformCommand(GameObject* obj, Transform oldT, Transform newT, glm::vec3 oldCol, glm::vec3 newCol)
    : m_Target(obj), m_OldTransform(oldT), m_NewTransform(newT), m_OldColor(oldCol), m_NewColor(newCol) {}

void TransformCommand::Execute() {
    if (m_Target) {
        m_Target->transform = m_NewTransform;
        m_Target->color = m_NewColor;
    }
}

void TransformCommand::Undo() {
    if (m_Target) {
        m_Target->transform = m_OldTransform;
        m_Target->color = m_OldColor;
    }
}

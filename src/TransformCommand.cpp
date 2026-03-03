#include "TransformCommand.h"
#include "GameObject.h" // Only include the full class here

TransformCommand::TransformCommand(GameObject* obj, Transform oldT, Transform newT)
    : m_Target(obj), m_OldTransform(oldT), m_NewTransform(newT) {}

void TransformCommand::Execute() {
    if (m_Target) {
        m_Target->transform = m_NewTransform;
    }
}

void TransformCommand::Undo() {
    if (m_Target) {
        m_Target->transform = m_OldTransform;
    }
}

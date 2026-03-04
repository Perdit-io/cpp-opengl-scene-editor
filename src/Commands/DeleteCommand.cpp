#include "DeleteCommand.h"
#include "Scene.h"
#include "GameObject.h"

DeleteCommand::DeleteCommand(Scene* scene, GameObject* obj)
    : m_Scene(scene), m_RawPtr(obj) {}

void DeleteCommand::Execute() {
    if (!m_HeldObject) {
        m_HeldObject = m_Scene->ExtractGameObject(m_RawPtr);
    }
}

void DeleteCommand::Undo() {
    if (m_HeldObject) {
        m_Scene->ReinsertGameObject(std::move(m_HeldObject));
    }
}

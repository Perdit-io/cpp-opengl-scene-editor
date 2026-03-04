#include "CreateCommand.h"
#include "Scene.h"
#include "GameObject.h"

CreateCommand::CreateCommand(Scene* scene, const std::string& name, Mesh* mesh)
    : m_Scene(scene), m_Name(name), m_Mesh(mesh), m_RawPtr(nullptr), m_HeldObject(nullptr) {
    (void)sizeof(GameObject);
}

void CreateCommand::Execute() {
    if (m_HeldObject) {
        m_Scene->ReinsertGameObject(std::move(m_HeldObject));
    } else {
        m_RawPtr = m_Scene->CreateGameObject(m_Name);
    }

    if (m_RawPtr) {
        m_RawPtr->mesh = m_Mesh;
    }
}

void CreateCommand::Undo() {
    if (m_RawPtr) {
        m_HeldObject = m_Scene->ExtractGameObject(m_RawPtr);
    }
}

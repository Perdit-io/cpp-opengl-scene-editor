#include "BatchParentCommand.h"
#include "GameObject.h"

void BatchCommand::Add(std::unique_ptr<Command> cmd) {
    m_Commands.push_back(std::move(cmd));
}

void BatchCommand::Execute() {
    for (auto& cmd : m_Commands) cmd->Execute();
}

void BatchCommand::Undo() {
    for (auto it = m_Commands.rbegin(); it != m_Commands.rend(); ++it) {
        (*it)->Undo();
    }
}

ParentCommand::ParentCommand(GameObject* child, GameObject* newParent)
    : m_Child(child), m_NewParent(newParent) {
        m_OldParent = child->GetParent();
    }

void ParentCommand::Execute() {
    m_Child->SetParent(m_NewParent);
}

void ParentCommand::Undo() {
    m_Child->SetParent(m_OldParent);
}

SetupCommand::SetupCommand(GameObject* obj, const Transform& transform)
    : m_Obj(obj), m_Transform(transform) {}

void SetupCommand::Execute() {
    m_Obj->transform = m_Transform;
}

void SetupCommand::Undo() {}

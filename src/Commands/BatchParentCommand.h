#pragma once

#include "Command.h"
#include "Transform.h"

#include <vector>
#include <memory>

class GameObject;

class BatchCommand : public Command {
public:
    void Add(std::unique_ptr<Command> cmd);
    void Execute() override;
    void Undo() override;

private:
    std::vector<std::unique_ptr<Command>> m_Commands;
};

class ParentCommand : public Command {
public:
    ParentCommand(GameObject* child, GameObject* newParent);
    void Execute() override;
    void Undo() override;

private:
    GameObject* m_Child;
    GameObject* m_NewParent;
    GameObject* m_OldParent;
};

class SetupCommand : public Command {
public:
    SetupCommand(GameObject* obj, const Transform& transform);
    void Execute() override;
    void Undo() override;

private:
    GameObject* m_Obj;
    Transform m_Transform;
};

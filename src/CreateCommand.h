#pragma once
#include "Command.h"

#include <memory>
#include <string>

class GameObject;
class Scene;

class CreateCommand : public Command {
public:
    CreateCommand(Scene* scene, const std::string& name);
    ~CreateCommand() = default;

    void Execute() override;
    void Undo() override;

private:
    Scene* m_Scene;
    std::string m_Name;
    GameObject* m_RawPtr;
    std::unique_ptr<GameObject> m_HeldObject;
};

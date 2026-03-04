#pragma once
#include "Command.h"

#include <memory>

class GameObject;
class Scene;

class DeleteCommand : public Command {
public:
    DeleteCommand(Scene* scene, GameObject* obj);
    ~DeleteCommand() = default;

    void Execute() override;
    void Undo() override;
private:
    Scene* m_Scene;
    GameObject* m_RawPtr;
    std::unique_ptr<GameObject> m_HeldObject;
};

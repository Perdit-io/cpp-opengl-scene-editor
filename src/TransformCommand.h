#pragma once
#include "Command.h"
#include "Transform.h"

class GameObject;

class TransformCommand : public Command {
public:
    TransformCommand(GameObject* obj, Transform oldT, Transform newT);

    void Execute() override;
    void Undo() override;

private:
    GameObject* m_Target;
    Transform m_OldTransform;
    Transform m_NewTransform;
};

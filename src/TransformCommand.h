#pragma once
#include "Command.h"
#include "Transform.h"

class GameObject;

class TransformCommand : public Command {
public:
    TransformCommand(GameObject* obj, Transform oldT, Transform newT, glm::vec3 oldCol, glm::vec3 newCol);

    void Execute() override;
    void Undo() override;

private:
    GameObject* m_Target;
    Transform m_OldTransform;
    Transform m_NewTransform;
    glm::vec3 m_OldColor;
    glm::vec3 m_NewColor;
};

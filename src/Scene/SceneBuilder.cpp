#include "SceneBuilder.h"
#include "Scene.h"
#include "CreateCommand.h"
#include "BatchParentCommand.h"
#include "GameObject.h"

GameObject* SceneBuilder::CreateObject(const std::string& name, Mesh* mesh, GameObject* parent,
                         glm::vec3 pos, glm::vec3 scale)
{
    auto createCmd = std::make_unique<CreateCommand>(scene, name, mesh);
    createCmd->Execute();
    GameObject* obj = scene->GetGameObjects().back().get();
    batch->Add(std::move(createCmd));

    if (parent) {
        auto parentCmd = std::make_unique<ParentCommand>(obj, parent);
        parentCmd->Execute();
        batch->Add(std::move(parentCmd));
    }

    obj->transform.position = pos;
    obj->transform.scale = scale;
    batch->Add(std::make_unique<SetupCommand>(obj, obj->transform));

    return obj;
}

#pragma once
#include <glm/glm.hpp>

class Scene;
class BatchCommand;
class GameObject;
class Mesh;

struct SceneBuilder {
    Scene* scene;
    BatchCommand* batch;

    GameObject* CreateObject(const std::string& name, Mesh* mesh, GameObject* parent = nullptr,
                             glm::vec3 pos = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));
};

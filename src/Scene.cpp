#include "Scene.h"
#include "GameObject.h"
#include <memory>

Scene::Scene() {
    m_Root = std::make_unique<GameObject>();
    m_Root->name = "Scene Root";
}

Scene::~Scene() {
    Clear();
}

GameObject* Scene::CreateGameObject(const std::string& name) {
    auto obj = std::make_unique<GameObject>();
    obj->name = name;
    GameObject* ptr = obj.get();
    m_Root->AddChild(ptr);
    m_GameObjects.push_back(std::move(obj));
    return ptr;
}

void Scene::Clear() {
    m_GameObjects.clear();
}

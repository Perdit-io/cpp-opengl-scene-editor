#include "Scene.h"
#include "GameObject.h"
#include <memory>

Scene::Scene() {
    m_Root = std::make_unique<GameObject>();
    m_Root->name = "Scene Root";
}

GameObject* Scene::CreateGameObject(const std::string& name) {
    auto obj = std::make_unique<GameObject>();
    obj->name = name;
    GameObject* ptr = obj.get();
    m_Root->AddChild(ptr);
    m_GameObjects.push_back(std::move(obj));
    return ptr;
}

void Scene::DestroyGameObject(GameObject* obj) {
    if (!obj || obj == m_Root.get()) return;

    GameObject* parent = obj->parent;
    if (parent) {
        auto& children = parent->children;
        children.erase(std::remove(children.begin(), children.end(), obj), children.end());
    }

    m_GameObjects.erase(
        std::remove_if(m_GameObjects.begin(), m_GameObjects.end(),
            [obj](const std::unique_ptr<GameObject>& g) { return g.get() == obj; }),
        m_GameObjects.end()
    );
}

std::unique_ptr<GameObject> Scene::ExtractGameObject(GameObject* obj) {
    if (!obj || obj == m_Root.get()) return nullptr;

    if (obj->parent) {
        auto& siblings = obj->parent->children;
        siblings.erase(std::remove(siblings.begin(), siblings.end(), obj), siblings.end());

        obj->parent = nullptr;
    }

    for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it) {
        if (it->get() == obj) {
            std::unique_ptr<GameObject> extracted = std::move(*it);
            m_GameObjects.erase(it);
            return extracted;
        }
    }
    return nullptr;
}

void Scene::ReinsertGameObject(std::unique_ptr<GameObject> obj) {
    if (!obj) return;

    GameObject* rawPtr = obj.get();
    m_Root->AddChild(rawPtr);

    m_GameObjects.push_back(std::move(obj));
}

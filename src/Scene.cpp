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
    ptr->SetParent(m_Root.get());
    m_GameObjects.push_back(std::move(obj));
    return ptr;
}

void Scene::DestroyGameObject(GameObject* obj) {
    if (!obj || obj == m_Root.get()) return;

    obj->SetParent(nullptr);

    m_GameObjects.erase(
        std::remove_if(m_GameObjects.begin(), m_GameObjects.end(),
            [obj](const std::unique_ptr<GameObject>& g) { return g.get() == obj; }),
        m_GameObjects.end()
    );
}

std::unique_ptr<GameObject> Scene::ExtractGameObject(GameObject* obj) {
    if (!obj || obj == m_Root.get()) return nullptr;

    obj->SetParent(nullptr);

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

    obj.get()->SetParent(m_Root.get());

    m_GameObjects.push_back(std::move(obj));
}

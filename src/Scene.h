#pragma once

#include <vector>
#include <memory>
#include <string>

class GameObject;

class Scene {
public:
    Scene();
    ~Scene() = default;

    GameObject* CreateGameObject(const std::string& name = "New Object");
    void DestroyGameObject(GameObject* obj);

    std::unique_ptr<GameObject> ExtractGameObject(GameObject* obj);
    void ReinsertGameObject(std::unique_ptr<GameObject> obj);

    GameObject* GetRoot() const { return m_Root.get(); }
private:
    std::unique_ptr<GameObject> m_Root;
    std::vector<std::unique_ptr<GameObject>> m_GameObjects;
};

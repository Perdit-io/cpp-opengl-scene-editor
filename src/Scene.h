#pragma once

#include <vector>
#include <memory>
#include <string>

class GameObject;

class Scene {
public:
    Scene();
    ~Scene();

    GameObject* CreateGameObject(const std::string& name = "New Object");
    void Clear();

    GameObject* GetRoot() const { return m_Root.get(); }
private:
    std::unique_ptr<GameObject> m_Root;
    std::vector<std::unique_ptr<GameObject>> m_GameObjects;
};

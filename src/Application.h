#pragma once

#include <memory>

struct GLFWwindow;
class Scene;
class Shader;
class Mesh;
class GameObject;

class Application {
public:
    Application(const char* title, int width, int height);
    ~Application();

    void Run();
private:
    bool InitWindow(const char* title, int width, int height);
    void InitImGui();
    void Cleanup();
    void DrawUI();
    void DrawSceneHierarchy(GameObject* node);
    void DrawInspector();
    void CleanupDeletedObjects(GameObject* node);

    GLFWwindow* m_Window;
    std::unique_ptr<Scene> m_ActiveScene;
    std::unique_ptr<Shader> m_MainShader;
    std::unique_ptr<Mesh> m_CubeMesh;
    std::unique_ptr<Mesh> m_PlaneMesh;
    GameObject* m_SelectedGameObject = nullptr;
};

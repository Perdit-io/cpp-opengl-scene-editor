#pragma once

#include <memory>
#include <deque>

struct GLFWwindow;
class Scene;
class Shader;
class Mesh;
class GameObject;
class Camera;
class Command;

class Application {
public:
    Application(const char* title, int width, int height);
    ~Application();

    void Run();
private:
    bool InitWindow(const char* title, int width, int height);
    void InitImGui();
    void Cleanup();
    void Update(float deltaTime);
    void DrawUI();
    void DrawSceneHierarchy(GameObject* node);
    void DrawInspector();
    void RenderScene();

    GLFWwindow* m_Window;
    std::unique_ptr<Scene> m_ActiveScene;
    std::unique_ptr<Shader> m_MainShader;
    std::unique_ptr<Mesh> m_CubeMesh;
    std::unique_ptr<Mesh> m_PlaneMesh;
    std::unique_ptr<Mesh> m_SphereMesh;
    GameObject* m_SelectedGameObject = nullptr;
    std::unique_ptr<Camera> m_Camera;

    std::deque<std::unique_ptr<Command>> m_UndoStack;
    std::deque<std::unique_ptr<Command>> m_RedoStack;
    const size_t MAX_UNDO_STEPS = 20;

    void PushCommand(std::unique_ptr<Command> cmd);
    void Undo();
    void Redo();
    void HandleInput(float deltaTime);

    void SpawnStackTestCase();
    void SpawnTableTestCase();
};

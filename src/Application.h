#pragma once

#include <memory>

struct GLFWwindow;
class Scene;
class Shader;
class Mesh;

class Application {
public:
    Application(const char* title, int width, int height);
    ~Application();

    void Run();
private:
    bool InitWindow(const char* title, int width, int height);
    void InitImGui();
    void Cleanup();

    GLFWwindow* m_Window;
    std::unique_ptr<Scene> m_ActiveScene;
    std::unique_ptr<Shader> m_MainShader;
    std::unique_ptr<Mesh> m_CubeMesh;
    std::unique_ptr<Mesh> m_PlaneMesh;
};

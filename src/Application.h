#pragma once

struct GLFWwindow;

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
};

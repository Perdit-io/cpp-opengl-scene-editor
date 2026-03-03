#include "config.h"
#include "Application.h"
#include "GameObject.h"
#include "Scene.h"
#include "Shader.h"
#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace {
    std::vector<Vertex> cubeVertices = {
        // Back face
        {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}}, {{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}}, {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}}, {{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}}, {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}},
        // Front face
        {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}}, {{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}}, {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}}, {{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}}, {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}},
        // Left face
        {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}}, {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}}, {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}}, {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}}, {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}},
        // Right face
        {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}}, {{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}}, {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}}, {{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}}, {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}},
        // Bottom face
        {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}}, {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}}, {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}}, {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}}, {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}},
        // Top face
        {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}}, {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}}, {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}}, {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}}, {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}}
    };

    std::vector<Vertex> planeVertices = {
        {{-10.0f, -0.5f, -10.0f}, {0.0f, 1.0f, 0.0f}}, {{ 10.0f, -0.5f, -10.0f}, {0.0f, 1.0f, 0.0f}}, {{ 10.0f, -0.5f,  10.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 10.0f, -0.5f,  10.0f}, {0.0f, 1.0f, 0.0f}}, {{-10.0f, -0.5f,  10.0f}, {0.0f, 1.0f, 0.0f}}, {{-10.0f, -0.5f, -10.0f}, {0.0f, 1.0f, 0.0f}}
    };
}

Application::Application(const char* title, int width, int height) {
    if (!InitWindow(title, width, height)) return;
    InitImGui();

    m_ActiveScene = std::make_unique<Scene>();
    m_MainShader = std::make_unique<Shader>("shaders/default.vert", "shaders/default.frag");
    m_CubeMesh = std::make_unique<Mesh>(cubeVertices);
    m_PlaneMesh = std::make_unique<Mesh>(planeVertices);

    m_ActiveScene->CreateGameObject("Cube 1")->transform.position = glm::vec3( 0.0f, 0.9f, 0.0f);
    m_ActiveScene->CreateGameObject("Cube 2")->transform.position = glm::vec3(-1.5f, 2.0f, 0.0f);
    m_ActiveScene->CreateGameObject("Cube 3")->transform.position = glm::vec3(-1.5f, 3.0f, -2.0f);
}

Application::~Application() {
    Cleanup();
}

bool Application::InitWindow(const char* title, int width, int height) {
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_Window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!m_Window) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_Window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return false;

    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(0);

    return true;
}

void Application::InitImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Application::Run() {
    double lastTime = glfwGetTime();
    const double targetFPS = 144.0;
    const double targetFrameTime = 1.0 / targetFPS;

    while (!glfwWindowShouldClose(m_Window)) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;

        if (deltaTime < targetFrameTime) continue;
        lastTime = currentTime;

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        DrawUI();

        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- RENDER START ---
        m_MainShader->Use();
        m_MainShader->SetVec3("lightPos", glm::vec3(1.5f, 5.0f, 3.0f));
        m_MainShader->SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        int display_w, display_h;
        glfwGetFramebufferSize(m_Window, &display_w, &display_h);
        float aspect = (float)display_w / (float)display_h;

        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -8.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        m_MainShader->SetMat4("view", view);
        m_MainShader->SetMat4("projection", projection);

        m_MainShader->SetVec3("objectColor", glm::vec3(0.2f, 0.2f, 0.2f));
        m_MainShader->SetMat4("model", glm::mat4(1.0f));
        m_PlaneMesh->Draw();

        m_MainShader->SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.2f));
        for (auto& obj : m_ActiveScene->GetRoot()->children) {
            m_MainShader->SetMat4("model", obj->GetWorldMatrix());
            m_CubeMesh->Draw();
        }
        // --- RENDER END ---

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(m_Window);

        CleanupDeletedObjects(m_ActiveScene->GetRoot());
    }
}

void Application::Cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Application::DrawUI() {
    ImGui::Begin("World Outliner");
    for (auto child : m_ActiveScene->GetRoot()->children) {
        DrawSceneHierarchy(child);
    }

    ImGui::Separator();
    static int cubeCount = 0;
    if (ImGui::Button("Add Cube")) {
        std::string name = "Cube " + std::to_string(++cubeCount);
        m_ActiveScene->CreateGameObject(name);
    }
    ImGui::End();

    DrawInspector();

    ImGui::Begin("Status");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void Application::DrawSceneHierarchy(GameObject* node) {
    ImGuiTreeNodeFlags flags = ((m_SelectedGameObject == node) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

    bool node_open = false;
    if (node->children.empty()) {
        flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx((void*)(intptr_t)node, flags, "%s", node->name.c_str());
    } else {
        node_open = ImGui::TreeNodeEx((void*)(intptr_t)node, flags, "%s", node->name.c_str());
    }

    if (ImGui::IsItemClicked()) m_SelectedGameObject = node;

    if (node->parent != nullptr && ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Delete Object")) {
            node->markedForDeletion = true; // Just mark it!
        }
        ImGui::EndPopup();
    }

    if (node_open) {
        for (auto child : node->children) {
            DrawSceneHierarchy(child);
        }
        ImGui::TreePop();
    }
}

void Application::DrawInspector() {
    ImGui::Begin("Inspector");
    if (m_SelectedGameObject) {
        ImGui::Text("Name: %s", m_SelectedGameObject->name.c_str());
        ImGui::Separator();

        ImGui::DragFloat3("Position", glm::value_ptr(m_SelectedGameObject->transform.position), 0.1f);
        ImGui::DragFloat3("Rotation", glm::value_ptr(m_SelectedGameObject->transform.rotation), 1.0f);
        ImGui::DragFloat3("Scale", glm::value_ptr(m_SelectedGameObject->transform.scale), 0.1f);

        ImGui::Separator();
        if (ImGui::Button("Reset Transform")) {
            m_SelectedGameObject->transform.position = glm::vec3(0.0f);
            m_SelectedGameObject->transform.rotation = glm::vec3(0.0f);
            m_SelectedGameObject->transform.scale = glm::vec3(1.0f);
        }
    } else {
        ImGui::Text("Select an object to edit.");
    }
    ImGui::End();
}

void Application::CleanupDeletedObjects(GameObject* node) {
    for (int i = 0; i < node->children.size(); ++i) {
        GameObject* child = node->children[i];

        if (child->markedForDeletion) {
            if (m_SelectedGameObject == child) m_SelectedGameObject = nullptr;

            m_ActiveScene->DestroyGameObject(child);

            --i;
        } else {
            CleanupDeletedObjects(child);
        }
    }
}

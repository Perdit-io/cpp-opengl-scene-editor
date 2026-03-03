#include "config.h"
#include "Application.h"
#include "GameObject.h"
#include "Scene.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Command.h"
#include "CreateCommand.h"
#include "TransformCommand.h"
#include "DeleteCommand.h"
#include "PrimitiveGenerator.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Application::Application(const char* title, int width, int height) {
    if (!InitWindow(title, width, height)) return;
    InitImGui();

    m_ActiveScene = std::make_unique<Scene>();
    m_MainShader = std::make_unique<Shader>("shaders/default.vert", "shaders/default.frag");

    // 1. Initialize Primitives
    m_CubeMesh   = std::make_unique<Mesh>(PrimitiveGenerator::GenerateCube());
    m_PlaneMesh  = std::make_unique<Mesh>(PrimitiveGenerator::GeneratePlane(20.0f));
    m_SphereMesh = std::make_unique<Mesh>(PrimitiveGenerator::GenerateSphere(1.0f, 32, 16));

    // 2. Setup initial scene (Test Case 1)
    auto floor = m_ActiveScene->CreateGameObject("Ground Plane");
    floor->mesh = m_PlaneMesh.get();
    floor->color = glm::vec3(0.2f, 0.2f, 0.2f);
    floor->transform.position = glm::vec3(0.0f, 0.0f, 0.0f);

    auto c1 = m_ActiveScene->CreateGameObject("Cube 1");
    c1->transform.position = glm::vec3(0.0f, 0.9f, 0.0f);
    c1->mesh = m_CubeMesh.get();

    auto c2 = m_ActiveScene->CreateGameObject("Cube 2");
    c2->transform.position = glm::vec3(-1.5f, 2.0f, 0.0f);
    c2->mesh = m_CubeMesh.get();

    auto c3 = m_ActiveScene->CreateGameObject("Cube 3");
    c3->transform.position = glm::vec3(-1.5f, 3.0f, -2.0f);
    c3->mesh = m_CubeMesh.get();

    m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 1.0f, 8.0f));
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

        Update((float)deltaTime);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        DrawUI();

        RenderScene();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_Window);
    }
}

void Application::Cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Application::Update(float deltaTime) {
    if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Z, false)) {
        Undo();
    }

    if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Y, false)) {
        Redo();
    }
}

void Application::DrawUI() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z", false, !m_UndoStack.empty())) {
                Undo();
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y", false, !m_RedoStack.empty())) {
                Redo();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::Begin("World Outliner");
    for (auto child : m_ActiveScene->GetRoot()->children) {
        DrawSceneHierarchy(child);
    }

    ImGui::Separator();

    if (ImGui::Button("Spawn Object...")) {
        ImGui::OpenPopup("spawn_popup");
    }

    if (ImGui::BeginPopup("spawn_popup")) {
        static int spawnCount = 0;

        auto Spawn = [&](const std::string& type, Mesh* meshPtr) {
            std::string name = type + " (" + std::to_string(++spawnCount) + ")";

            // The command now takes the mesh as a parameter
            auto cmd = std::make_unique<CreateCommand>(m_ActiveScene.get(), name, meshPtr);
            PushCommand(std::move(cmd));
        };

        if (ImGui::MenuItem("Cube"))   Spawn("Cube", m_CubeMesh.get());
        if (ImGui::MenuItem("Sphere")) Spawn("Sphere", m_SphereMesh.get());
        if (ImGui::MenuItem("Plane"))  Spawn("Plane", m_PlaneMesh.get());

        ImGui::EndPopup();
    }

    ImGui::End();

    // --- 3. Inspector & Status ---
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
            auto cmd = std::make_unique<DeleteCommand>(m_ActiveScene.get(), node);

            if (m_SelectedGameObject == node) m_SelectedGameObject = nullptr;

            PushCommand(std::move(cmd));
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

        static Transform transformBeforeEdit;
        static glm::vec3 colorBeforeEdit;
        static bool isEditingColor = false;

        auto DrawUndoableDrag = [&](const char* label, glm::vec3& data, float speed) {
            ImGui::DragFloat3(label, glm::value_ptr(data), speed);
            if (ImGui::IsItemActivated()) {
                transformBeforeEdit = m_SelectedGameObject->transform;
                colorBeforeEdit = m_SelectedGameObject->color;
            }
            if (ImGui::IsItemDeactivatedAfterEdit()) {
                PushCommand(std::make_unique<TransformCommand>(
                    m_SelectedGameObject,
                    transformBeforeEdit, m_SelectedGameObject->transform,
                    colorBeforeEdit, m_SelectedGameObject->color
                ));
            }
        };

        DrawUndoableDrag("Position", m_SelectedGameObject->transform.position, 0.1f);
        DrawUndoableDrag("Rotation", m_SelectedGameObject->transform.rotation, 1.0f);
        DrawUndoableDrag("Scale", m_SelectedGameObject->transform.scale, 0.1f);

        ImGui::ColorEdit3("Object Color", &m_SelectedGameObject->color.x);

        if (ImGui::IsItemActive() && !isEditingColor) {
            transformBeforeEdit = m_SelectedGameObject->transform;
            colorBeforeEdit = m_SelectedGameObject->color;
            isEditingColor = true;
        }

        if (isEditingColor && !ImGui::IsItemActive() && !ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopupId)) {
            if (colorBeforeEdit != m_SelectedGameObject->color) {
                PushCommand(std::make_unique<TransformCommand>(
                    m_SelectedGameObject,
                    transformBeforeEdit, m_SelectedGameObject->transform,
                    colorBeforeEdit, m_SelectedGameObject->color
                ));
            }
            isEditingColor = false;
        }

        ImGui::Separator();

        if (ImGui::Button("Reset Properties")) {
            Transform oldT = m_SelectedGameObject->transform;
            glm::vec3 oldC = m_SelectedGameObject->color;

            Transform newT;
            glm::vec3 newC = glm::vec3(1.0f, 0.5f, 0.2f);

            m_SelectedGameObject->transform = newT;
            m_SelectedGameObject->color = newC;

            PushCommand(std::make_unique<TransformCommand>(
                m_SelectedGameObject, oldT, newT, oldC, newC
            ));
        }
    } else {
        ImGui::Text("Select an object to edit.");
    }
    ImGui::End();
}

void Application::RenderScene() {
    // 1. Clear buffers
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2. Setup Global Uniforms
    m_MainShader->Use();
    m_MainShader->SetVec3("lightPos", glm::vec3(1.5f, 5.0f, 3.0f));
    m_MainShader->SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    int display_w, display_h;
    glfwGetFramebufferSize(m_Window, &display_w, &display_h);
    float aspect = (float)display_w / (float)display_h;

    m_MainShader->SetMat4("view", m_Camera->GetViewMatrix());
    m_MainShader->SetMat4("projection", m_Camera->GetProjectionMatrix(aspect));

    // 3. Render Loop
    for (auto& obj : m_ActiveScene->GetGameObjects()) {
        if (obj->mesh) {
            m_MainShader->SetVec3("objectColor", obj->color);
            m_MainShader->SetMat4("model", obj->GetWorldMatrix());
            obj->mesh->Draw();
        }
    }
}

void Application::PushCommand(std::unique_ptr<Command> cmd) {
    cmd->Execute();

    m_UndoStack.push_back(std::move(cmd));
    m_RedoStack.clear();

    if (m_UndoStack.size() > MAX_UNDO_STEPS) {
        m_UndoStack.pop_front();
    }
}

void Application::Undo() {
    if (m_UndoStack.empty()) return;

    auto cmd = std::move(m_UndoStack.back());
    m_UndoStack.pop_back();

    cmd->Undo();
    m_RedoStack.push_back(std::move(cmd));
}

void Application::Redo() {
    if (m_RedoStack.empty()) return;

    auto cmd = std::move(m_RedoStack.back());
    m_RedoStack.pop_back();

    cmd->Execute();
    m_UndoStack.push_back(std::move(cmd));
}

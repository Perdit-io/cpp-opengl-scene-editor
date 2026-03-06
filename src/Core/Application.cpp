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
#include "TextureCommand.h"
#include "PrimitiveGenerator.h"
#include "BatchParentCommand.h"
#include "SceneBuilder.h"
#include "MeshLoader.h"
#include "Texture.h"

#include <cstddef>
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

    auto floor = m_ActiveScene->CreateGameObject("Ground Plane");
    floor->mesh = m_PlaneMesh.get();
    floor->color = glm::vec3(0.2f, 0.2f, 0.2f);
    floor->transform.position = glm::vec3(0.0f, 0.0f, 0.0f);

    m_FloorTexture = std::make_unique<Texture>("assets/textures/floor.jpg", "diffuse");
    floor->texture = m_FloorTexture.get();
    m_TextureFallback = std::make_unique<Texture>("assets/textures/floor.jpg", "diffuse");

    m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 7.0f, 35.0f));
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
    glEnable(GL_BLEND);
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
    HandleInput(deltaTime);
    if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Z, false)) {
        Undo();
    }

    if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Y, false)) {
        Redo();
    }
}

void Application::HandleInput(float deltaTime) {
    static bool firstMouse = true;
    static float lastX = 0.0f, lastY = 0.0f;

    if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Keyboard
        if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS) m_Camera->ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS) m_Camera->ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS) m_Camera->ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS) m_Camera->ProcessKeyboard(RIGHT, deltaTime);
        if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)      m_Camera->ProcessKeyboard(UP, deltaTime);
        if (glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) m_Camera->ProcessKeyboard(DOWN, deltaTime);

        // Mouse
        double xpos, ypos;
        glfwGetCursorPos(m_Window, &xpos, &ypos);

        if (firstMouse) {
            lastX = (float)xpos;
            lastY = (float)ypos;
            firstMouse = false;
        }

        float xoffset = (float)xpos - lastX;
        float yoffset = lastY - (float)ypos;
        lastX = (float)xpos;
        lastY = (float)ypos;

        m_Camera->ProcessMouse(xoffset, yoffset);
    } else {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstMouse = true; // Reset so it doesn't jump when we re-click
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
    for (auto child : m_ActiveScene->GetRoot()->GetChildren()) {
        DrawSceneHierarchy(child);
    }

    ImGui::Separator();

    if (ImGui::Button("Spawn Primitive")) {
        ImGui::OpenPopup("spawn_popup");
    }

    if (ImGui::BeginPopup("spawn_popup")) {
        auto Spawn = [&](const std::string& type, Mesh* meshPtr) {
            m_SelectedGameObject = nullptr;

            std::string name = type + " (" + std::to_string(++m_SpawnCount) + ")";

            // The command now takes the mesh as a parameter
            auto cmd = std::make_unique<CreateCommand>(m_ActiveScene.get(), name, meshPtr);
            PushCommand(std::move(cmd));
        };

        if (ImGui::MenuItem("Cube"))   Spawn("Cube", m_CubeMesh.get());
        if (ImGui::MenuItem("Sphere")) Spawn("Sphere", m_SphereMesh.get());
        if (ImGui::MenuItem("Plane"))  Spawn("Plane", m_PlaneMesh.get());

        ImGui::EndPopup();
    }

    if (ImGui::Button("Create Empty")) {
        m_SelectedGameObject = nullptr;

        std::string name = "New Empty Object (" + std::to_string(++m_SpawnCount) + ")";
        auto cmd = std::make_unique<CreateCommand>(m_ActiveScene.get(), name, nullptr);
        PushCommand(std::move(cmd));
    }

    if (ImGui::Button("Import/Spawn")) { ImGui::OpenPopup("ImportSpawnPopup"); }

    if (ImGui::BeginPopup("ImportSpawnPopup")) {
        static char objPath[256] = "assets/models/suzanne.obj";
        static float importScale = 1.0f;

        ImGui::Text("Import Settings");
        ImGui::InputText("OBJ Path", objPath, 256);

        ImGui::InputFloat("Import Scale", &importScale, 0.001f, 0.01f, "%.4f");

        ImGui::Separator();

        if (ImGui::Button("Import OBJ")) {
            ModelData modelData = MeshLoader::LoadOBJ(objPath);

            std::string objDir = std::string(objPath).substr(0, std::string(objPath).find_last_of("/\\") + 1);
            std::string parentDir = objDir.substr(0, objDir.find_last_of("/\\", objDir.length() - 2) + 1);

            if (!modelData.subMeshes.empty()) {
                auto batch = std::make_unique<BatchCommand>();
                SceneBuilder builder = { m_ActiveScene.get(), batch.get() };
                GameObject* rootObj = builder.CreateObject("Sponza_Root", nullptr, nullptr, nullptr,
                                                          glm::vec3(0.0f), glm::vec3(importScale));

                for (auto& sm : modelData.subMeshes) {
                    auto newMesh = std::make_unique<Mesh>(sm.vertices);
                    m_LoadedMeshes.push_back(std::move(newMesh));

                    Texture* texPtr = nullptr;
                    if (modelData.materialTextures.count(sm.materialName)) {
                        std::string relPath = modelData.materialTextures[sm.materialName];

                        std::string finalPath = parentDir + relPath;

                        auto newTex = std::make_unique<Texture>(finalPath, "diffuse");
                        texPtr = newTex.get();
                        m_LoadedTextures.push_back(std::move(newTex));
                    }

                    builder.CreateObject(sm.name, m_LoadedMeshes.back().get(), texPtr, rootObj);
                }

                m_UndoStack.push_back(std::move(batch));
                m_RedoStack.clear();

                if (m_UndoStack.size() > MAX_UNDO_STEPS) {
                    m_UndoStack.pop_front();
                }
            }
        }

        ImGui::Separator();

        // Test Case Spawners
        if (ImGui::MenuItem("Spawn Test Case 1 (Cube Stack)")) {
            m_SelectedGameObject = nullptr;
            SpawnStackTestCase();
        }
        if (ImGui::MenuItem("Spawn Test Case 2 (Table)")) {
            m_SelectedGameObject = nullptr;
            SpawnTableTestCase();
        }
        if (ImGui::MenuItem("Spawn Test Case 3 (Sponza Environment)")) {
            m_SelectedGameObject = nullptr;
            SpawnSponzaTestCase();
        }

        ImGui::EndPopup();
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
    if (node->GetChildren().empty()) {
        flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx((void*)(intptr_t)node, flags, "%s", node->name.c_str());
    } else {
        node_open = ImGui::TreeNodeEx((void*)(intptr_t)node, flags, "%s", node->name.c_str());
    }

    if (ImGui::IsItemClicked()) m_SelectedGameObject = node;

    if (node->GetParent() != nullptr && ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Delete Object")) {
            auto cmd = std::make_unique<DeleteCommand>(m_ActiveScene.get(), node);

            if (m_SelectedGameObject == node) m_SelectedGameObject = nullptr;

            PushCommand(std::move(cmd));
        }
        ImGui::EndPopup();
    }

    if (node_open) {
        for (auto child : node->GetChildren()) {
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
        ImGui::Text("Material Properties");

        // Display current texture path if available
        std::string currentPath = m_SelectedGameObject->texture ? m_SelectedGameObject->texture->GetPath() : "None";
        ImGui::Text("Current: %s", currentPath.c_str());

        // Buffer for new texture path
        static char texPathBuffer[256] = "assets/textures/";
        ImGui::InputText("New Texture Path", texPathBuffer, 256);

        if (ImGui::Button("Apply Texture")) {
            std::string pathStr(texPathBuffer);

            Texture* existingTex = nullptr;
            for (auto& tex : m_LoadedTextures) {
                if (tex->GetPath() == pathStr) {
                    existingTex = tex.get();
                    break;
                }
            }

            if (existingTex) {
                m_SelectedGameObject->texture = existingTex;
            } else {
                auto newTex = std::make_unique<Texture>(pathStr, "diffuse");
                if (newTex->GetID() != 0) {
                    existingTex = newTex.get();
                    m_SelectedGameObject->texture = newTex.get();
                    m_LoadedTextures.push_back(std::move(newTex));
                }
            }

            auto cmd = std::make_unique<TextureCommand>(m_SelectedGameObject, existingTex);
            PushCommand(std::move(cmd));
        }

        if (ImGui::Button("Remove Texture")) {
            m_SelectedGameObject->texture = nullptr;
        }

        ImGui::Separator();

        ImGui::Text("Relationship");

        GameObject* sceneRoot = m_ActiveScene->GetRoot();
        GameObject* currentParent = m_SelectedGameObject->GetParent();

        const char* parentName = currentParent ? currentParent->name.c_str() : "Scene Root";

        if (ImGui::BeginCombo("Parent", parentName)) {
            bool isRootSelected = (currentParent == sceneRoot);
            if (ImGui::Selectable("Scene Root", isRootSelected)) {
                m_SelectedGameObject->SetParent(sceneRoot);
            }

            for (auto& obj : m_ActiveScene->GetGameObjects()) {
                if (obj.get() == m_SelectedGameObject || obj.get() == sceneRoot) continue;
                if (obj->IsDescendantOf(m_SelectedGameObject)) continue;

                bool isSelected = (currentParent == obj.get());
                if (ImGui::Selectable(obj->name.c_str(), isSelected)) {
                    m_SelectedGameObject->SetParent(obj.get());
                }
            }
            ImGui::EndCombo();
            if (currentParent) {
                glm::vec3 pScale = currentParent->transform.scale;
                const float epsilon = 0.001f;
                bool isNonUniform = (std::abs(pScale.x - pScale.y) > epsilon) ||
                                    (std::abs(pScale.y - pScale.z) > epsilon) ||
                                    (std::abs(pScale.x - pScale.z) > epsilon);

                if (isNonUniform) {
                    ImGui::Spacing();
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.2f, 1.0f)); // Warning Orange/Yellow
                    ImGui::TextWrapped("(!) Warning: Non-uniform parent scale detected.");
                    ImGui::PopStyleColor();

                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f)); // Muted Gray
                    ImGui::TextWrapped("Rotating this child may result in skew/shear distortion because the parent's axes are not equal in length.");
                    ImGui::PopStyleColor();
                }
            }
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
    m_MainShader->SetInt("texture_diffuse", 0);
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
            if (obj->texture && obj->texture->GetID() != 0) {
                m_MainShader->SetBool("useTexture", true);
                obj->texture->Bind(0);
            } else {
                // If texture failed or is null, use the fallback and set useTexture to false
                m_MainShader->SetBool("useTexture", false);
                m_TextureFallback->Bind(0);
            }
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

    m_SelectedGameObject = nullptr;

    auto cmd = std::move(m_UndoStack.back());
    m_UndoStack.pop_back();

    cmd->Undo();
    m_RedoStack.push_back(std::move(cmd));
}

void Application::Redo() {
    if (m_RedoStack.empty()) return;

    m_SelectedGameObject = nullptr;

    auto cmd = std::move(m_RedoStack.back());
    m_RedoStack.pop_back();

    cmd->Execute();
    m_UndoStack.push_back(std::move(cmd));
}

void Application::SpawnStackTestCase() {
    auto batch = std::make_unique<BatchCommand>();
    SceneBuilder builder = { m_ActiveScene.get(), batch.get() };

    builder.CreateObject("Cube 1", m_CubeMesh.get(), nullptr, nullptr, glm::vec3(0.0f, 0.9f, 0.0f));

    builder.CreateObject("Cube 2", m_CubeMesh.get(), nullptr, nullptr, glm::vec3(-1.5f, 2.0f, 0.0f));

    builder.CreateObject("Cube 3", m_CubeMesh.get(), nullptr, nullptr, glm::vec3(-1.5f, 3.0f, -2.0f));

    m_UndoStack.push_back(std::move(batch));
    m_RedoStack.clear();

    if (m_UndoStack.size() > MAX_UNDO_STEPS) {
        m_UndoStack.pop_front();
    }
}

void Application::SpawnTableTestCase() {
    auto batch = std::make_unique<BatchCommand>();
    SceneBuilder builder = { m_ActiveScene.get(), batch.get() };

    // 1. Create Group
    GameObject* group = builder.CreateObject("Table_Assembly", nullptr, nullptr, nullptr, glm::vec3(10.0f, 0.0f, 0.0f));

    // 2. Create Top
    builder.CreateObject("Table_Top", m_CubeMesh.get(), nullptr, group,
                         glm::vec3(0.0f, 4.75f, 0.0f), glm::vec3(10.0f, 1.5f, 5.0f));

    // 3. Create Legs
    float offsetX = 4.75f;
    float offsetZ = 2.25f;
    glm::vec3 positions[] = {
        {  offsetX, 2.0f,  offsetZ }, { -offsetX, 2.0f,  offsetZ },
        {  offsetX, 2.0f, -offsetZ }, { -offsetX, 2.0f, -offsetZ }
    };

    for (int i = 0; i < 4; ++i) {
        builder.CreateObject("Leg_" + std::to_string(i + 1), m_CubeMesh.get(), nullptr, group,
                             positions[i], glm::vec3(0.5f, 4.0f, 0.5f));
    }

    m_UndoStack.push_back(std::move(batch));
    m_RedoStack.clear();

    if (m_UndoStack.size() > MAX_UNDO_STEPS) {
        m_UndoStack.pop_front();
    }
}

void Application::SpawnSponzaTestCase() {
    std::string path = "assets/models/sponza.obj";
    float scale = 0.05f;

    ModelData modelData = MeshLoader::LoadOBJ(path);

    std::string objDir = path.substr(0, path.find_last_of("/\\") + 1);
    std::string parentDir = objDir.substr(0, objDir.find_last_of("/\\", objDir.length() - 2) + 1);

    auto batch = std::make_unique<BatchCommand>();
    SceneBuilder builder = { m_ActiveScene.get(), batch.get() };

    GameObject* rootObj = builder.CreateObject("Sponza_Environment", nullptr, nullptr, nullptr,
                                               glm::vec3(0.0f), glm::vec3(scale));

    for (auto& sm : modelData.subMeshes) {
        auto newMesh = std::make_unique<Mesh>(sm.vertices);
        m_LoadedMeshes.push_back(std::move(newMesh));

        Texture* texPtr = nullptr;
        if (modelData.materialTextures.count(sm.materialName)) {
            std::string relPath = modelData.materialTextures[sm.materialName];
            std::string finalPath = parentDir + relPath;

            for (auto& tex : m_LoadedTextures) {
                if (tex->GetPath() == finalPath) {
                    texPtr = tex.get();
                    break;
                }
            }

            if (!texPtr) {
                auto newTex = std::make_unique<Texture>(finalPath, "diffuse");
                if (newTex->GetID() != 0) {
                    texPtr = newTex.get();
                    m_LoadedTextures.push_back(std::move(newTex));
                }
            }
        }

        builder.CreateObject(sm.name, m_LoadedMeshes.back().get(), texPtr, rootObj);
    }

    m_UndoStack.push_back(std::move(batch));
    m_RedoStack.clear();

    if (m_UndoStack.size() > MAX_UNDO_STEPS) {
        m_UndoStack.pop_front();
    }
}

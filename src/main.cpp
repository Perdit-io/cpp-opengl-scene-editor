#include "Application.h"

int main() {
    Application app("Scene Editor", 1280, 720);
    app.Run();
    return 0;

//     // 1. Setup GLFW
//     if (!glfwInit()) return -1;
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
// #ifdef __APPLE__
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif

//     GLFWwindow* window = glfwCreateWindow(800, 600, "Simple Window", NULL, NULL);
//     if (!window) { glfwTerminate(); return -1; }
//     glfwMakeContextCurrent(window);

//     // 2. Setup GLAD
//     if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
//         glfwTerminate();
//         return -1;
//     }

//     // 3. Setup ImGui
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGui_ImplGlfw_InitForOpenGL(window, true);
//     ImGui_ImplOpenGL3_Init("#version 330");

//     // 4. Main Loop
//     while (!glfwWindowShouldClose(window)) {
        // glfwPollEvents();

        // ImGui_ImplOpenGL3_NewFrame();
        // ImGui_ImplGlfw_NewFrame();
        // ImGui::NewFrame();

        // ImGui::Begin("Status");
        // ImGui::Text("Everything is working!");
        // ImGui::End();

        // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);

        // ImGui::Render();
        // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // glfwSwapBuffers(window);
//     }

//     // Cleanup
//     ImGui_ImplOpenGL3_Shutdown();
//     ImGui_ImplGlfw_Shutdown();
//     ImGui::DestroyContext();
//     glfwTerminate();
//     return 0;
}

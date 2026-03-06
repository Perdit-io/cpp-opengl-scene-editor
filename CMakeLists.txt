cmake_minimum_required(VERSION 3.15)
project(SceneEditor VERSION 1.0.0 LANGUAGES CXX C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Fetch GLFW
include(FetchContent)
FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(glfw)

# 2. Setup GLAD
add_library(glad "${CMAKE_SOURCE_DIR}/dependencies/glad/src/glad.c")
target_include_directories(glad PUBLIC
    "${CMAKE_SOURCE_DIR}/dependencies/glad/include"
)

# 3. Setup ImGui
file(GLOB IMGUI_SOURCES
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/*.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/backends/imgui_impl_glfw.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/backends/imgui_impl_opengl3.cpp")
add_library(imgui ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC
    "${CMAKE_SOURCE_DIR}/dependencies/imgui"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/backends"
)
target_link_libraries(imgui PRIVATE glfw glad)

# 4. Main Executable
add_executable(SceneEditor
    # Core
    src/main.cpp
    src/Core/Application.cpp
    src/Core/Camera.cpp

    # Scene
    src/Scene/GameObject.cpp
    src/Scene/Scene.cpp
    src/Scene/SceneBuilder.cpp

    # Graphics
    src/Graphics/Shader.cpp
    src/Graphics/Mesh.cpp
    src/Graphics/PrimitiveGenerator.cpp
    src/Graphics/Texture.cpp
    src/Graphics/stb_image_impl.cpp

    # Commands
    src/Commands/CreateCommand.cpp
    src/Commands/TransformCommand.cpp
    src/Commands/DeleteCommand.cpp
    src/Commands/BatchParentCommand.cpp
    src/Commands/TextureCommand.cpp

    # Assets
    src/Assets/MeshLoader.cpp
)

# 5. Include Directories
target_include_directories(SceneEditor PRIVATE
    "${CMAKE_SOURCE_DIR}/src/Core"
    "${CMAKE_SOURCE_DIR}/src/Scene"
    "${CMAKE_SOURCE_DIR}/src/Graphics"
    "${CMAKE_SOURCE_DIR}/src/Commands"
    "${CMAKE_SOURCE_DIR}/src/Assets"
    "${CMAKE_SOURCE_DIR}/dependencies/glad/include"
    "${CMAKE_SOURCE_DIR}/dependencies/glm"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui"
    "${CMAKE_SOURCE_DIR}/dependencies/stb"
    "${CMAKE_SOURCE_DIR}/dependencies/tinyobjloader"
)

target_link_libraries(SceneEditor PRIVATE glfw glad imgui)

# 6. OS Specifics
if(APPLE)
    target_link_libraries(SceneEditor PRIVATE
        "-framework OpenGL"
        "-framework IOKit"
        "-framework Cocoa"
    )
endif()

# 7. Resource Management
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

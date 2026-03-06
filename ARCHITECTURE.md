# Scene Editor Architecture

## Tech Stack
* **Language:** C++17
* **Graphics:** OpenGL 3.3 Core
* **Windowing:** GLFW 3.3
* **UI:** Dear ImGui v1.92.6-docking
* **Math:** GLM 1.0.3
* **Assets:** tinyobjloader v1.0.6 & stb_image v2.30

## Key Systems
### Transform Hierarchy
To support parenting, objects use a tree-based Scene Graph. 
`GlobalTransform = ParentGlobal * LocalTransform`.

### Command Pattern
Undo/Redo is implemented via a Command Pattern. Every editor action (Move, Spawn, Delete) is encapsulated in a class with `Execute()` and `Undo()` methods.

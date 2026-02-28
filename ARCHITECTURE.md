# Scene Editor Architecture

## Tech Stack
* **Language:** C++17
* **Graphics:** OpenGL 3.3 Core
* **Windowing:** GLFW 3.3
* **UI:** Dear ImGui
* **Math:** GLM
* **Assets:** tinyobjloader & stb_image

## Key Systems
### Transform Hierarchy
To support parenting, objects use a tree-based Scene Graph. 
`GlobalTransform = ParentGlobal * LocalTransform`.

### Command Pattern
Undo/Redo is implemented via a Command Pattern. Every editor action (Move, Spawn, Delete) is encapsulated in a class with `Execute()` and `Undo()` methods.

A real-time 3D scene composition tool in C++ featuring a hierarchical scene graph, command-based undo/redo, and automated asset pipelining via **tinyobjloader** and **stb_image**.

### Build Instructions

1. Ensure **CMake (3.15+)** and a C++ compiler are installed.
2. `mkdir build && cd build`
3. `cmake ..`
4. `make`
5. `./SceneEditor`

---

### Controls

* **[W,A,S,D,Shift,Space]**: Move Camera
* **[Right Click + Mouse]**: Look Around
* **[Left Click]**: Select Object (In World Outliner)
* **[Ctrl+Z / Ctrl+Y]**: Undo/Redo

---

### Core Architecture

* **Scene Graph**: Implements a tree-based parent-child relationship system for complex object assemblies.
* **Command Pattern**: Encapsulates all editor actions (Transform, Texture, Create, Delete) to provide a robust, non-destructive history stack.
* **Asset Pipeline**: Utilizes **tinyobjloader** for hierarchical mesh parsing and **stb_image** for automated MTL material mapping and texture caching.

---

### Test Cases

* **Test Case 1**: Cube Stack (Physics/Stacking layout).
* **Test Case 2**: Procedural Table Assembly (Parenting & Scale).
* **Test Case 3**: Sponza Atrium (High-poly environment & Automated Materials).

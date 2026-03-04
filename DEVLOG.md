# Development Log - Scene Editor

## Milestone 13: OBJ Model Loader & Diagnostics
**Goal:** Load and render external .obj geometry (as a single mesh) with real-time parsing feedback and performance benchmarking.

## Milestone 12.5: Project Organization
**Goal:** Modularize the codebase into specialized subdirectories

## Milestone 12: Hierarchy & Scene Organization
**Goal:** Establish a robust parent-child system capable of handling complex primitive assemblies.

## Milestone 11: Editor Navigation (Fly-Camera)
**Goal:** Implement a 3D flying camera system with WASD movement and mouse-look to allow for spatial inspection and object alignment.

## Milestone 10: Primitive Factory & Property System
**Goal:** Abstract mesh generation via `PrimitiveGenerator` and implement Sphere/Plane primitives with a robust, undoable property system for colors and transforms.

## Milestone 9: Undo/Redo (The Command Pattern)
**Goal:** Implement a robust history system using the Command Pattern to manage object lifecycle (Create/Delete) and property modifications (Transform).

## Milestone 8.5: Architecture Polish & Refactoring
**Goal:** Consolidate matrix math, abstract the camera system, and isolate rendering logic to prepare for complex input handling.

## Milestone 8: Editor UI & Object Management
**Goal:** Implement a functional scene editor interface with a hierarchy outliner, property inspector, and safe object lifecycle management (Spawn/Delete).

## Milestone 7: Basic Lighting (Lambertian Shading)
**Goal:** Implement vertex normals and a basic lighting model (ambient + diffuse) to provide 3D depth and visual volume.

## Milestone 6: Geometry & Scene Layout
**Goal:** Implement the physical layout for Test Case 1, including 3D cube primitives, a ground plane, and aspect-ratio corrected perspective rendering.

## Milestone 5: Shader System
**Goal:** Implement automated GLSL compilation and cross-platform shader loading logic.

## Milestone 4: Scene Management
**Goal:** Create a central container to manage the lifecycle of all GameObjects

## Milestone 3: Transform & Hierarchy
**Goal:** Implement the math for 3D positioning and the parent-child relationship.

## Milestone 2.5: Architecture Refactor
**Goal:** Encapsulated the engine lifecycle into a modular `Application` class for a clean `main.cpp`.

## Milestone 2: The Skeleton
**Goal:** Initialize the windowing system, OpenGL context, and UI docking framework.

## Milestone 1: Project Foundation & Documentation
**Goal:** Define project scope, requirements, and architectural patterns.

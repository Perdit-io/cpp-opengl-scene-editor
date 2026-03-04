#pragma once
#include <vector>
#include <memory>

class Mesh;

class Model {
public:
    std::vector<std::unique_ptr<Mesh>> meshes;

    void Draw();
};

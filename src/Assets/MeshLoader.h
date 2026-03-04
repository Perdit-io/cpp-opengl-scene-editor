#pragma once

#include <string>

class Mesh;

class MeshLoader {
public:
    static Mesh* LoadOBJ(const std::string& path);
};

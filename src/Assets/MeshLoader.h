#pragma once

#include <string>
#include <vector>

struct Vertex;

struct SubMeshData {
    std::string name;
    std::vector<Vertex> vertices;
};

class MeshLoader {
public:
    static std::vector<SubMeshData> LoadOBJ(const std::string& path);
};

#pragma once

#include <string>
#include <vector>
#include <map>

struct Vertex;

struct SubMeshData {
    std::string name;
    std::string materialName;
    std::vector<Vertex> vertices;
    int materialId;
};

struct ModelData {
    std::vector<SubMeshData> subMeshes;
    std::map<int, std::string> materialTextures;
};

class MeshLoader {
public:
    static ModelData LoadOBJ(const std::string& path);
};

#pragma once

#include <string>
#include <vector>
#include <map>

struct Vertex;

struct SubMeshData {
    std::string name;
    std::string materialName;
    std::vector<Vertex> vertices;
};

struct ModelData {
    std::vector<SubMeshData> subMeshes;
    std::map<std::string, std::string> materialTextures;
};

class MeshLoader {
public:
    static ModelData LoadOBJ(const std::string& path);
private:
    static std::map<std::string, std::string> ParseMTL(const std::string& path);
};

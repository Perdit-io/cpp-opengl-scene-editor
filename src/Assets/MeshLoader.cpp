#include "MeshLoader.h"
#include "Mesh.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

ModelData MeshLoader::LoadOBJ(const std::string& path) {
    ModelData data;

    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec3> temp_normals;
    std::vector<glm::vec2> temp_uvs;

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open OBJ file at: " << path << std::endl;
        return data;
    }

    double startTime = glfwGetTime();
    std::cout << "--- Starting Import: " << path << " ---" << std::endl;

    std::string directory = path.substr(0, path.find_last_of("/\\") + 1);
    std::string line;
    SubMeshData currentMesh;
    currentMesh.name = "Initial_Mesh";

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "mtllib") {
            std::string mtlFile;
            ss >> mtlFile;
            data.materialTextures = ParseMTL(directory + mtlFile);
        }
        else if (prefix == "usemtl") {
            if (!currentMesh.vertices.empty()) {
                data.subMeshes.push_back(currentMesh);
                currentMesh.vertices.clear();
            }
            ss >> currentMesh.materialName;
        }
        else if (prefix == "o" || prefix == "g") {
            // If the current mesh has data, save it before starting a new one
            if (!currentMesh.vertices.empty()) {
                data.subMeshes.push_back(currentMesh);
                currentMesh.vertices.clear();
            }
            ss >> currentMesh.name;
        }
        else if (prefix == "v") {
            glm::vec3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            temp_positions.push_back(pos);
        }
        else if (prefix == "vt") {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            temp_uvs.push_back(uv);
        }
        else if (prefix == "vn") {
            glm::vec3 norm;
            ss >> norm.x >> norm.y >> norm.z;
            temp_normals.push_back(norm);
        }
        else if (prefix == "f") {
            for (int i = 0; i < 3; i++) {
                std::string vertexData;
                ss >> vertexData;

                // Check for the double-slash indicating missing UVs (v//vn)
                bool hasNoUV = (vertexData.find("//") != std::string::npos);

                std::replace(vertexData.begin(), vertexData.end(), '/', ' ');
                std::stringstream vss(vertexData);

                int vIdx, vtIdx = -1, vnIdx = -1;
                vss >> vIdx;

                Vertex v;
                v.Position = temp_positions[vIdx - 1];

                if (hasNoUV) {
                    vss >> vnIdx; // v//vn format: skip vtIdx and read vnIdx
                    v.Normal = temp_normals[vnIdx - 1];
                    v.TexCoords = glm::vec2(0.0f);
                } else {
                    // v/vt/vn or v/vt format
                    if (vss >> vtIdx) {
                        v.TexCoords = temp_uvs[vtIdx - 1];
                    }
                    if (vss >> vnIdx) {
                        v.Normal = temp_normals[vnIdx - 1];
                    }
                }
                currentMesh.vertices.push_back(v);
            }
        }
    }

    if (!currentMesh.vertices.empty()) {
        data.subMeshes.push_back(currentMesh);
    }

    double endTime = glfwGetTime();
    double duration = endTime - startTime;

    std::cout << "--- Import Complete ---" << std::endl;
    std::cout << "  Total Vertices: " << temp_positions.size() << std::endl;
    std::cout << "  Total Faces:    " << data.subMeshes.size() << std::endl;
    std::cout << "  Time Taken:     " << duration << " seconds" << std::endl;
    std::cout << "-----------------------" << std::endl;

    return data;
}

std::map<std::string, std::string> MeshLoader::ParseMTL(const std::string& path) {
    std::map<std::string, std::string> materialMap;
    std::ifstream file(path);
    if (!file.is_open()) return materialMap;

    std::string line, currentMat, prefix;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        ss >> prefix;
        if (prefix == "newmtl") ss >> currentMat;
        else if (prefix == "map_Kd") {
            std::string texPath;
            ss >> texPath;
            materialMap[currentMat] = texPath;
        }
    }
    return materialMap;
}

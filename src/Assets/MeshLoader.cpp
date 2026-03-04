#include "MeshLoader.h"
#include "Model.h"
#include "Mesh.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

std::vector<SubMeshData> MeshLoader::LoadOBJ(const std::string& path) {
    std::vector<SubMeshData> subMeshes;
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec3> temp_normals;
    std::vector<glm::vec2> temp_uvs;

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open OBJ file at: " << path << std::endl;
        return subMeshes;
    }

    double startTime = glfwGetTime();
    std::cout << "--- Starting Import: " << path << " ---" << std::endl;

    std::string line;
    SubMeshData currentMesh;
    currentMesh.name = "Initial_Mesh";

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "o" || prefix == "g") {
            // If the current mesh has data, save it before starting a new one
            if (!currentMesh.vertices.empty()) {
                subMeshes.push_back(currentMesh);
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

                std::replace(vertexData.begin(), vertexData.end(), '/', ' ');
                std::stringstream vss(vertexData);

                int vIdx, vtIdx = -1, vnIdx = -1;
                vss >> vIdx;

                Vertex v;
                v.Position = temp_positions[vIdx - 1];

                bool hasNoUV = (vertexData.find("  ") != std::string::npos);

                if (hasNoUV) {
                    vss >> vnIdx;
                    v.Normal = temp_normals[vnIdx - 1];
                    v.TexCoords = glm::vec2(0.0f);
                } else {
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
        subMeshes.push_back(currentMesh);
    }

    double endTime = glfwGetTime();
    double duration = endTime - startTime;

    std::cout << "--- Import Complete ---" << std::endl;
    std::cout << "  Total Vertices: " << temp_positions.size() << std::endl;
    std::cout << "  Total Faces:    " << currentMesh.vertices.size() / 3 << std::endl;
    std::cout << "  Time Taken:     " << duration << " seconds" << std::endl;
    std::cout << "-----------------------" << std::endl;

    return subMeshes;
}

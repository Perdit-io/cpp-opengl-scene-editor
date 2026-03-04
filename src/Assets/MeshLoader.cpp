#include "MeshLoader.h"
#include "Mesh.h"

#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

Mesh* MeshLoader::LoadOBJ(const std::string& path) {
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec3> temp_normals;
    std::vector<Vertex> out_vertices;

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open OBJ file at: " << path << std::endl;
        return nullptr;
    }

    // --- 1. Start Timer ---
    double startTime = glfwGetTime();
    std::cout << "--- Starting Import: " << path << " ---" << std::endl;

    int vertexCount = 0;
    int objectCount = 0;
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        // --- 2. Group/Object Logger ---
        if (prefix == "o" || prefix == "g") {
            std::string name;
            ss >> name;
            objectCount++;
            std::cout << "  [OBJ] Found Object/Group: " << name << std::endl;
        }
        else if (prefix == "v") {
            glm::vec3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            temp_positions.push_back(pos);

            // --- 3. Progress Logger ---
            vertexCount++;
            if (vertexCount % 50000 == 0) {
                std::cout << "  [Log] Parsed " << vertexCount << " vertices..." << std::endl;
            }
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

                int vIdx, vtIdx, vnIdx;
                vss >> vIdx;

                Vertex v;
                v.Position = temp_positions[vIdx - 1];

                if (vertexData.find("  ") != std::string::npos) {
                    vss >> vnIdx;
                    v.Normal = temp_normals[vnIdx - 1];
                } else if (vss >> vtIdx >> vnIdx) {
                    v.Normal = temp_normals[vnIdx - 1];
                }
                out_vertices.push_back(v);
            }
        }
    }

    // --- 4. End Timer & Summary ---
    double endTime = glfwGetTime();
    double duration = endTime - startTime;

    std::cout << "--- Import Complete ---" << std::endl;
    std::cout << "  Total Vertices: " << temp_positions.size() << std::endl;
    std::cout << "  Total Faces:    " << out_vertices.size() / 3 << std::endl;
    std::cout << "  Sub-objects:    " << objectCount << " (Flattened into 1 mesh)" << std::endl;
    std::cout << "  Time Taken:     " << duration << " seconds" << std::endl;
    std::cout << "-----------------------" << std::endl;

    return new Mesh(out_vertices);
}

#include "MeshLoader.h"
#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <GLFW/glfw3.h>
#include <iostream>

ModelData MeshLoader::LoadOBJ(const std::string& path) {
    ModelData data;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    std::string baseDir = "";
    size_t lastSlash = path.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        baseDir = path.substr(0, lastSlash + 1);
    }

    double startTime = glfwGetTime();
    std::cout << "--- Starting Import (TinyOBJ v1.0.6): " << path << " ---" << std::endl;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str(), baseDir.c_str(), true);

    if (!err.empty()) std::cerr << "TinyOBJ Message: " << err << std::endl;
    if (!ret) return data;

    for (size_t i = 0; i < materials.size(); i++) {
        if (!materials[i].diffuse_texname.empty()) {
            data.materialTextures[(int)i] = materials[i].diffuse_texname;
        }
    }

    for (const auto& shape : shapes) {
        SubMeshData subMesh;
        subMesh.name = shape.name;

        subMesh.materialId = shape.mesh.material_ids.empty() ? -1 : shape.mesh.material_ids[0];
        if (subMesh.materialId >= 0) {
            subMesh.materialName = materials[subMesh.materialId].name;
        }

        for (const auto& index : shape.mesh.indices) {
            Vertex v;

            v.Position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            if (index.normal_index >= 0) {
                v.Normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }

            if (index.texcoord_index >= 0) {
                v.TexCoords = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            subMesh.vertices.push_back(v);
        }
        data.subMeshes.push_back(subMesh);
    }

    std::cout << "--- Import Complete ---" << std::endl;
    std::cout << "  Sub-meshes: " << data.subMeshes.size() << std::endl;
    std::cout << "  Time Taken: " << (glfwGetTime() - startTime) << "s" << std::endl;

    return data;
}

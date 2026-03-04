#include "Model.h"
#include "Mesh.h"

void Model::Draw() {
    for(auto& mesh : meshes) {
        mesh->Draw();
    }
}

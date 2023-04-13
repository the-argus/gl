#pragma once
#include <assimp/scene.h>

#include "mesh.hpp"
#include "shader.hpp"

class Model {
    public:
        Model (char* path) {
            loadModel(path);
        }
        void Draw(Shader &shader);
    private:
        std::vector<Mesh> meshes;
        std::string directory;

        void loadModel(std::string path);
        void processNode(aiMesh *node, const aiScene *scene);
};

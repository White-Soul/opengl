#pragma once
#include <string>
#include <vector>
#include "Shader.h"
#include "Mesh.h"
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
unsigned int TextureFromFile(const char* path, const std::string& directory,
                             bool gamma = false);
class Model {
  public:
    Model(const std::string& path, bool gamma = false)
        : gammaCorrection(gamma) {
        loadModel(path);
    }
    Model(const char* path, bool gamma = false) : gammaCorrection(gamma) {
        loadModel(path);
    }
    void Draw(Shader& shader);

  private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;
    bool gammaCorrection;

    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat,
                                              aiTextureType type,
                                              std::string typeName);
};

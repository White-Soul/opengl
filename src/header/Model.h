#pragma once
#include <string>
#include <vector>
#include "Shader.h"
#include "Mesh.h"
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
/**
 * @brief 获取纹理
 *
 * @param path 纹理路径
 * @param directory 目录
 * @param gamma 伽玛校正
 * @return unsigned int 纹理名称
 */
unsigned int TextureFromFile(const char* path, const std::string& directory,
                             bool gamma = false);
/**
 * @brief 模型
 * @class
 */
class Model {
  public:
    /**
     * @brief 构造一个模型对象
     *
     * @param path 模型路径
     * @param gamma 伽玛校正--默认为false
     */
    Model(const std::string& path, bool gamma = false)
        : gammaCorrection(gamma) {
        loadModel(path);
    }
    /**
     * @brief 构造一个模型对象
     *
     * @param path 模型路径
     * @param gamma 伽玛校正--默认为false
     */
    Model(const char* path, bool gamma = false) : gammaCorrection(gamma) {
        loadModel(path);
    }
    /**
     * @brief 绘制模型及其所有网格
     *
     * @param shader 着色器对象
     */
    void Draw(Shader& shader);

    // 提供外部接口访问数据可能非必要
  public:
    inline std::vector<Mesh>& getMesh() { return meshes; }
    inline std::string getDirectory() const { return directory; }
    inline std::vector<Texture>& getTexturesLoaded() { return textures_loaded; }
    inline bool getGamma() const { return gammaCorrection; }

  private:
    // 网格数组
    std::vector<Mesh> meshes;
    // 模型所在目录
    std::string directory;
    // 缓存已经载入过的纹理
    std::vector<Texture> textures_loaded;
    // 伽玛校正
    bool gammaCorrection;
    /**
     * @brief 加载模型
     * 
     * @param path 模型路径
     */
    void loadModel(const std::string& path);
    /**
     * @brief 以递归方式处理节点。
     * 处理位于节点上的每个网格，并在其子节点（如果有）上重复此过程。
     * @param node aiNode指针
     * @param scene aiScene指针
     */
    void processNode(aiNode* node, const aiScene* scene);
    /**
     * @brief 处理每一个节点的网格，封装为Mesh对象
     * 
     * @param mesh aiMesh指针
     * @param scene aiScene指针
     * @return Mesh 网格对象
     */
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    /**
     * @brief 检查给定类型的所有材质纹理，如果尚未加载纹理，则加载纹理。
     * 所需信息作为纹理结构返回。
     * @param mat aiMaterial的指针
     * @param type aiTextureType对象
     * @param typeName 类型名称
     * @return std::vector<Texture> 纹理数组
     */
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat,
                                              aiTextureType type,
                                              std::string typeName);
};

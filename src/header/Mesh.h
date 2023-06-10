#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"
#define MAX_BONE_INFLUENCE 4
/**
 * @brief 顶点
 * @struct
 */
struct Vertex {
    // 顶点坐标
    glm::vec3 Position;
    // 法向量
    glm::vec3 Normal;
    // 纹理坐标
    glm::vec2 TexCoords;
    // 切线
    glm::vec3 Tangent;
    // 副切线
    glm::vec3 Bitangent;
    // 将影响此顶点的骨骼索引
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    // 每块骨头的重量
    float m_Weights[MAX_BONE_INFLUENCE];
};

/**
 * @brief 纹理
 * @struct
 */
struct Texture {
    // 纹理名称
    unsigned int id;
    // 纹理类型
    std::string type;
    // 纹理路径
    std::string path;
};

/**
 * @brief 网格
 * @class
 */
class Mesh {
  private:
    // 渲染数据
    unsigned int VAO, VBO, EBO;
    /**
     * @brief 初始化所有缓冲区对象/数组
     *
     */
    void setupMesh();
    // 网格数据
    // 顶点数据
    std::vector<Vertex> vertices;
    // 索引数据
    std::vector<unsigned int> indices;
    // 纹理数据
    std::vector<Texture> textures;

    // 提供外部接口访问数据
  public:
    inline std::vector<Vertex>& getVertices() { return vertices; }
    inline std::vector<unsigned int>& getIndices() { return indices; }
    inline std::vector<Texture>& getTextures() { return textures; }
    inline unsigned int getVAO() const { return VAO; }
    inline unsigned int getVBO() const { return VBO; }
    inline unsigned int getEBO() const { return EBO; }

  public:
    /**
     * @brief 渲染网格
     *
     * @param shader 着色器对象
     */
    void Draw(Shader& shader);
    /**
     * @brief 构造函数
     *
     * @param vertices 顶点数据
     * @param indices 索引数据
     * @param textures 纹理数据
     */
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
         std::vector<Texture> textures);
};

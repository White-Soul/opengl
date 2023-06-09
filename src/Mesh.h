#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"
#define MAX_BONE_INFLUENCE 4
// 顶点
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

// 纹理
struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

// 网格
class Mesh {
  private:
    // 渲染数据
    unsigned int VAO, VBO, EBO;
    // 函数
    void setupMesh();
    // 网格数据
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

  public:
    void Draw(Shader& shader);
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
         std::vector<Texture> textures);
};

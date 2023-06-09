#pragma once
#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
/**
 * @brief shader文件类型
 */
enum ShaderType { Vert, Frag };

/**
 * @brief shader类用于读取并编译使用shader
 */
class Shader {
  private:
    unsigned int ID;
    std::vector<std::pair<std::string, std::string>> shaderList;
    /**
     * @brief 读取shader文件, 文件必须为.vert/.frag结尾
     * @param path 文件路径
     */
    void readShader(std::string path);

  public:
    Shader(std::initializer_list<std::string> list);
    template <typename T>
    Shader(T path) {
        static_assert(std::is_convertible_v<T, std::string>,
                      "Path is not string");
        readShader(path);
    }
    template <typename T, typename... Args>
    Shader(T path, Args... paths) : Shader(paths...) {
        static_assert(std::is_convertible_v<T, std::string>,
                      "Path is not string");
        readShader(path);
    }
    ~Shader() {
        if (ID != 0) glDeleteProgram(ID);
    }
    void use();
    // uniform
    void setUniform(const std::string& name, bool value) const;
    void setUniform(const std::string& name, int value) const;
    void setUniform(const std::string& name, float value) const;
    void setUniform(const std::string& name, glm::mat4 val,
                    int count = 1) const;
    void setUniform(const std::string& name, glm::vec3 val) const;
    void Compile();
    void addShader(std::string path);
    unsigned int id() const { return ID; }
};

#endif
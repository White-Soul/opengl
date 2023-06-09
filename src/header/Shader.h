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
 * @class
 * @brief shader类用于读取并编译使用shader
 */
class Shader {
  private:
    /// @brief Shader名称
    unsigned int ID;
    /// @brief 文件名--文件内容 键值对列表
    std::vector<std::pair<std::string, std::string>> shaderList;
    /**
     * @brief 读取shader文件, 文件必须为.vert/.frag结尾
     * 
     * @param path 路径
     */
    void readShader(const std::string& path);

  public:
  /**
   * @brief 构造一个新的Shader对象
   * 
   * @param list 着色器文件路径列表
   */
    Shader(std::initializer_list<std::string> list);
    /**
     * @brief 构造一个新的Shader对象
     * 
     * @tparam T 文件的路径类型--必须能转换为std::string
     * @param path 文件路径
     */
    template <typename T>
    Shader(T path) {
        static_assert(std::is_convertible_v<T, std::string>,
                      "Path is not string");
        readShader(path);
    }
    /**
     * @brief 构造一个新的Shader对象
     * 
     * @tparam T 类型--必须能转换为std::string
     * @tparam Args 类型包--其中每一个必须都能转换为std::string
     * @param path 路径
     * @param paths 路径包
     */
    template <typename T, typename... Args>
    Shader(T path, Args... paths) : Shader(paths...) {
        static_assert(std::is_convertible_v<T, std::string>,
                      "Path is not string");
        readShader(path);
    }
    /**
     * @brief Destroy the Shader object
     * 
     */
    ~Shader() {
        if (ID != 0) glDeleteProgram(ID);
    }
    /**
     * @brief 使用着色器
     * 
     */
    void use();
    /**
     * @brief 设置着色器中的Uniform对象
     * 
     * @param name Uniform对象名称
     * @param value 需要设置的int值
     */
    void setUniform(const std::string& name, bool value) const;
    /**
     * @brief 设置着色器中的Uniform对象
     * 
     * @param name Uniform对象名称
     * @param value 需要设置的int值
     */
    void setUniform(const std::string& name, int value) const;
    /**
     * @brief 设置着色器中的Uniform对象
     * 
     * @param name Uniform对象名称
     * @param value 需要设置的float值
     */
    void setUniform(const std::string& name, float value) const;
    /**
     * @brief 设置着色器中的Uniform对象
     * 
     * @param name Uniform对象名称
     * @param val 需要设置的mat4值
     * @param count 需要加载数据的数组元素的数量或者需要修改的矩阵的数量--默认为1
     */
    void setUniform(const std::string& name, glm::mat4 val,
                    int count = 1) const;
    /**
     * @brief 设置着色器中的Uniform对象
     * 
     * @param name Uniform对象名称
     * @param val 需要设置的vec3值
     */
    void setUniform(const std::string& name, glm::vec3 val) const;
    /**
     * @brief 编译shader
     *
     */
    void Compile();
    /**
     * @brief 向shader中添加着色器文件
     *
     * @param path 着色器文件路径
     */
    void addShader(const std::string& path);
    /**
     * @brief
     *
     * @return unsigned int
     */
    unsigned int id() const { return ID; }
};

#endif
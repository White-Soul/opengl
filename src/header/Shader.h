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
#include "except.h"
#include "type_traits.hpp"
#include "defined.h"
#include "utils.h"
MGL_START
HAS_METHOD(length);
/*
.vert - 顶点着色器
.tesc - 曲面细分控制着色器
.tese - 曲面细分评估着色器
.geom - 几何着色器
.frag - 片段着色器
.comp - 计算着色器
*/
namespace ShaderFileType {
extern const char* Vert;
extern const char* Frag;
extern const char* Tesc;
extern const char* Tese;
extern const char* Geom;
extern const char* Comp;
};  // namespace ShaderFileType
/**
 * @brief shader文件类型
 */
enum class ShaderType { Unknown, Vert, Frag, Tesc, Tese, Geom, Comp };

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
     * @brief 读取shader文件, 文件类型必须是ShaderType中定义的类型
     *
     * @param path 路径
     */
    void readShader(const std::string& path);
    /**
     * @brief 获取着色器类型对应的OpenGL值
     *
     * @param type 着色器类型
     * @return int 对应的Opengl值
     */
    int getGLShaderType(ShaderType type) const;

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
    ~Shader() noexcept {
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
    /**
     * @brief 获取着色器文件对应的ShaderType
     *
     * @param path 着色器文件的路径
     * @return ShaderType 对应的ShaderType
     */
    ShaderType getShaderType(const std::string& path) const;
    /**
     * @brief 设置着色器中的Uniform对象
     *
     * @tparam 模板参数
     * @param name Uniform对象名称
     * @param val 需要设置的值
     * @param count 需要设置的数量
     */
    template <typename V>
    void setUniformV(const std::string& name, V val, unsigned int count = 1) {
        int num = val.count();
        switch (num) {
            case 2:
                glUniform2fv(glGetUniformLocation(this->ID, name.c_str()),
                             count, val.data());
                break;
            case 3:
                glUniform3fv(glGetUniformLocation(this->ID, name.c_str()),
                             count, val.data());
                break;
            case 4:
                glUniform4fv(glGetUniformLocation(this->ID, name.c_str()),
                             count, val.data());
                break;

            default:
                break;
        }
    }
    template <>
    void setUniformV<glm::vec2>(const std::string& name, glm::vec2 val,
                                unsigned int count) {
        float data[] = {val.x, val.y};
        glUniform2fv(glGetUniformLocation(this->ID, name.c_str()), count, data);
    }
    template <>
    void setUniformV<glm::vec3>(const std::string& name, glm::vec3 val,
                                unsigned int count) {
        float data[] = {val.x, val.y, val.z};
        glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), count, data);
    }
    template <>
    void setUniformV<glm::vec4>(const std::string& name, glm::vec4 val,
                                unsigned int count) {
        float data[] = {val.x, val.y, val.z, val.w};
        glUniform4fv(glGetUniformLocation(this->ID, name.c_str()), count,
                     data);
    }
    /**
     * @brief 设置着色器中的Uniform对象
     *
     * @tparam Matrix
     * @param name Uniform对象名称
     * @param val 需要设置的值
     * @param count 需要设置的数量默认1个
     */
    template <class M>
    void setUniformM(const std::string& name, M val, int count = 1) {
        int num = val.count();
        switch (num) {
            case 2:
                glUniformMatrix2fv(glGetUniformLocation(this->ID, name.c_str()),
                                   count, GL_FALSE, val.data());
                break;
            case 3:
                glUniformMatrix3fv(glGetUniformLocation(this->ID, name.c_str()),
                                   count, GL_FALSE, val.data());
                break;
            case 4:
                glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()),
                                   count, GL_FALSE, val.data());
                break;

            default:
                break;
        }
    }
    template <>
    void setUniformM<glm::mat4>(const std::string& name, glm::mat4 val,
                                int count) {
        glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), count,
                           GL_FALSE, glm::value_ptr(val));
    }
    template <>
    void setUniformM<glm::mat3>(const std::string& name, glm::mat3 val,
                                int count) {
        glUniformMatrix3fv(glGetUniformLocation(this->ID, name.c_str()), count,
                           GL_FALSE, glm::value_ptr(val));
    }
    template <>
    void setUniformM<glm::mat2>(const std::string& name, glm::mat2 val,
                                int count) {
        glUniformMatrix2fv(glGetUniformLocation(this->ID, name.c_str()), count,
                           GL_FALSE, glm::value_ptr(val));
    }
};
MGL_END
#endif
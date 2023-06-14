#include "header/Shader.h"
using namespace std;
Shader::Shader(initializer_list<string> list) {
    for (auto a = list.begin(); a != list.end(); ++a) {
        readShader(*a);
    }
}

void Shader::readShader(const string& path) {
    auto type = getShaderType(path);
    if (type == ShaderType::Unknown) {
        throw shader_exception(
            FileError, "Shader file is error, The file type cannot be judged");
    }
    string code;
    ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(path);
        stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        code = shaderStream.str();
    } catch (ifstream::failure e) {
        throw shader_exception(FileError,
                               "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
    }
    shaderList.push_back(make_pair(path, code));
}

void Shader::addShader(const string& path) { readShader(path); }

void Shader::Compile() {
    vector<unsigned int> ids;
    int success;
    char infoLog[512];
    for (auto a = shaderList.begin(); a != shaderList.end(); ++a) {
        unsigned int id;
        auto gl = getGLShaderType(getShaderType(a->first));
        id = glCreateShader(gl);
        const char* code = a->second.c_str();
        glShaderSource(id, 1, &code, NULL);
        glCompileShader(id);
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            throw shader_exception(CompileError, infoLog);
        };
        ids.push_back(id);
    }
    this->ID = glCreateProgram();
    for (auto id : ids) {
        glAttachShader(this->ID, id);
    }
    glLinkProgram(this->ID);
    glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
        throw shader_exception(LinkError, infoLog);
    }
    for (auto id : ids) {
        glDeleteShader(id);
    }
}

void Shader::setUniform(const string& name, bool val) const {
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)val);
}

void Shader::setUniform(const string& name, int val) const {
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), val);
}

void Shader::setUniform(const string& name, float val) const {
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), val);
}

void Shader::setUniform(const string& name, glm::mat4 val, int count) const {
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), count,
                       GL_FALSE, glm::value_ptr(val));
}

void Shader::setUniform(const std::string& name, glm::vec3 val) const {
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), val.x, val.y,
                val.z);
}

void Shader::use() { glUseProgram(this->ID); }

ShaderType Shader::getShaderType(const std::string& path) const {
    std::string str = path.substr(path.size() - 5, 5);
    ShaderType type{ShaderType::Unknown};
    if (str == ShaderFileType::Vert)
        type = ShaderType::Vert;
    else if (str == ShaderFileType::Frag)
        type = ShaderType::Frag;
    else if (str == ShaderFileType::Comp)
        type = ShaderType::Comp;
    else if (str == ShaderFileType::Tesc)
        type = ShaderType::Tesc;
    else if (str == ShaderFileType::Tese)
        type = ShaderType::Tese;
    else if (str == ShaderFileType::Geom)
        type = ShaderType::Geom;
    return type;
}

int Shader::getGLShaderType(ShaderType type) const {
    int gl_x;
    switch (type) {
        case ShaderType::Vert:
            gl_x = GL_VERTEX_SHADER;
            break;
        case ShaderType::Frag:
            gl_x = GL_FRAGMENT_SHADER;
            break;
        case ShaderType::Tesc:
            gl_x = GL_TESS_CONTROL_SHADER;
            break;
        case ShaderType::Tese:
            gl_x = GL_TESS_EVALUATION_SHADER;
            break;
        case ShaderType::Geom:
            gl_x = GL_GEOMETRY_SHADER;
            break;
        case ShaderType::Comp:
            gl_x = GL_COMPUTE_SHADER;
            break;

        default:
            break;
    }
    return gl_x;
}

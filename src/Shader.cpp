#include "Shader.h"
using namespace std;
Shader::Shader(initializer_list<string> list) {
    for (auto a = list.begin(); a != list.end(); ++a) {
        readShader(*a);
    }
}

void Shader::readShader(string path) {
    if (path.find(".vert") == -1 && path.find(".frag") == -1) {
        cout << "Shader file is error, The file type cannot be judged";
        std::exit(-1);
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
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
    }
    shaderList.push_back(make_pair(path, code));
}

void Shader::addShader(string path) { readShader(path); }

void Shader::Compile() {
    vector<unsigned int> ids;
    int success;
    char infoLog[512];
    for (auto a = shaderList.begin(); a != shaderList.end(); ++a) {
        unsigned int id;
        auto gl = a->first.find(".vert") == -1 ? GL_FRAGMENT_SHADER
                                               : GL_VERTEX_SHADER;
        id = glCreateShader(gl);
        const char* code = a->second.c_str();
        glShaderSource(id, 1, &code, NULL);
        glCompileShader(id);
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
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
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
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
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), val.x, val.y, val.z);
}

void Shader::use() { glUseProgram(this->ID); }
#include "header/except.h"

std::string shader_exception::getCodeStr(shader_code code) const {
    std::string str;
    switch (code) {
        case LinkError:
            str = "Shader link error";
            break;
        case CompileError:
            str = "Shader compilation error";
            break;
        case FileError:
            str = "Shader file error";
            break;

        default:
            str = "Unknown error";
            break;
    }
    return str;
}

const char* shader_exception::what() const noexcept {
    auto str = getCodeStr(_code) + ": " + _message;
    return _message.c_str();
}

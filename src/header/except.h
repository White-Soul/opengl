#pragma once
#include <exception>
#include <string>

enum shader_code { LinkError, CompileError, FileError };

class shader_exception : public std::exception {
  private:
    shader_code _code;
    std::string _message;

    std::string getCodeStr(shader_code code) const;

  public:
    shader_exception(shader_code code, const std::string &str)
        : _code(code), _message(str) {}

    virtual const char *what() const noexcept override;
};

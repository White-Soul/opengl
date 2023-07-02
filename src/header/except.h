#pragma once
#include <exception>
#include <string>
#include "defined.h"
MGL_START

enum shader_code { LinkError, CompileError, FileError };

class shader_exception : public std::exception {
  private:
    shader_code _code;
    std::string _message;

    std::string getCodeStr(shader_code code) const;

  public:
    shader_exception(shader_code code, const std::string &str)
        : _code(code), _message(str) {}
    shader_exception(const shader_exception &lval)
        : _code(lval._code), _message(lval._message) {}
#if __cplusplus >= 201103L
    shader_exception(shader_exception &&rval) _code(rval._code),
        _message(rval._message) {}
    shader_exception &operator=(shader_exception &&rval) {
        _code = rval._code;
        _message = rval._message;
        return *this;
    }
#endif
    shader_exception& operator=(const shader_exception& lval) = default;

    virtual const char *what() const noexcept override;
};

class bad_any_cast : public std::exception {
  public:
    bad_any_cast(const char *str): _message(str) {}
    bad_any_cast(const std::string &str): _message(str) {}
    bad_any_cast(const bad_any_cast &lval) = default;
#if __cplusplus >= 201103L
    bad_any_cast(bad_any_cast &&rval) = default;
    bad_any_cast &operator=(bad_any_cast &&rval) {
        _message = rval._message;
        return *this;
    }
#endif
    bad_any_cast &operator=(const bad_any_cast &lval) = default;

    virtual const char *what() const noexcept override;

  private:
    std::string _message;
};

MGL_END

#ifndef _MATRIX_TEMPLATE_
#define _MATRIX_TEMPLATE_

#include "defined.h"
MGL_START
template <class M>
class Matrix {
  public:
    Matrix() {}
    Matrix(const M &val) : _data(val) {}
    ~Matrix() {}
    M getData() const { return _data; }
    void setData(const M &val) { _data = val; }
    size_t count() const {
        return _data.count();
    }

  private:
    M _data;
    size_t _row;
    size_t _col;
};
MGL_END

#endif
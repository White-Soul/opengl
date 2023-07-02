#ifndef _VECTOR_TEMPLATE_
#define _VECTOR_TEMPLATE_

#include <vector>
#include "defined.h"
MGL_START

template <class T, int N>
class Vector {
  public:
    Vector() : _data(nullptr), _count(0){};
    ~Vector() {
        if (_data) delete[] _data;
    }
    Vector(T val) {
        _count = val.length();
        _data = new float[_count];
        for (int i = 0; i < _count; ++i) {
            _data[i] = val[i];
        }
    }
    Vector(const Vector& lval) {
        _count = lval._count;
        _data = new float[_count];
        std::memcpy(_data, lval.buffer, _count);
    }
    Vector(Vector&& rval) : _data(rval._data), _count(rval._count) {
        rval._data = nullptr;
        rval._count = 0;
    }
    float* getData() const { return _data; }
    void setData(T val) {
        _count = val.length();
        if (_data) _data = new float[val.count()];
        for (int i = 0; i < _count; ++i) {
            _data[i] = val[i];
        }
    }
    size_t count() const { return _count; }

  private:
    float* _data;
    size_t _count;
};

MGL_END
#endif
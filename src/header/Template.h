#pragma once
#ifndef _TEMPLATE_FUNC_
#define _TEMPLATE_FUNC_
#include <type_traits>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

template <typename Vector, typename Matrix>
Matrix Ortho(float left, float right, float bottom, float top, float zNear,
             float zFar);
template <typename Vector, typename Matrix>
Matrix Perspective(float fov, float aspect, float zNear, float zFar);
template <typename Vector, typename Matrix>
Matrix LookAt(const Vector& eye, const Vector& center, const Vector& up);
template <typename Vector, typename Matrix>
Vector Normalized(Vector v);
template <typename Vector, typename Matrix>
Vector Cross(Vector v1, Vector v2);
template <typename M>
const float* getMatrixData(const M& m);
#endif
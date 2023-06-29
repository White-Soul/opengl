#ifndef _TEMPLATE_FUNC_
#define _TEMPLATE_FUNC_
#include <type_traits>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * @brief 通过对应的值获取正交矩阵
 *
 * @param left 左范围
 * @param right 右范围
 * @param bottom 下范围
 * @param top 上范围
 * @param zNear 近范围
 * @param zFar 远范围
 * @return Matrix 对应的正交矩阵
 */
template <typename Vector, typename Matrix>
Matrix Ortho(float left, float right, float bottom, float top, float zNear,
             float zFar) {
    if (std::is_same_v<Matrix, glm::mat4>)
        return glm::ortho(left, right, bottom, top, zNear, zFar);
    else {
        Matrix mscale, mtranslate;
        mscale << (2.0 / (right - left)), 0, 0, 0, 0, (2.0 / (top - bottom)), 0,
            0, 0, 0, (2.0 / (zNear - zFar)), 0, 0, 0, 0, 1;
        mtranslate << 1, 0, 0, -(left + right) / 2, 0, 1, 0,
            -(bottom + top) / 2, 0, 0, 1, -(zNear + zFar) / 2, 0, 0, 0, 1;
        return mscale * mtranslate;
    }
}
/**
 * @brief 使用对应的值来获取投影矩阵
 *
 * @param fov 视角大小（弧度）
 * @param aspect 宽高比
 * @param zNear 近平面距离
 * @param zFar 远平面距离
 * @return M 对应的投影矩阵
 */
template <typename Vector, typename Matrix>
Matrix Perspective(float fov, float aspect, float zNear, float zFar) {
    if (std::is_same_v<Matrix, glm::mat4>)
        glm::perspective(fov, aspect, zNear, zFar);
    else {
        Matrix persp;
        persp << 1 / ((std::tan(fov / 2) * aspect)), 0, 0, 0, 0,
            1 / (std::tan(fov / 2)), 0, 0, 0, 0,
            (zNear + zFar) / (zNear - zFar),
            -(2 * zNear * zFar) / (zNear - zFar), 0, 0, 1, 0;
        return persp;
    }
}

/**
 * @brief 获取LookAt矩阵
 *
 * @param eye 相机位置
 * @param center 目标位置
 * @param up 上向量
 * @return Matrix LookAt矩阵
 */
template <typename Vector, typename Matrix>
Matrix LookAt(const Vector& eye, const Vector& center, const Vector& up) {
    if (std::is_same_v<Matrix, glm::mat4>)
        glm::lookAt(eye, center, up);
    else {
        Vector f(center - eye);
        Vector s(f.cross(up));
        Vector u(s.cross(f));
        f = Normalized(f);
        s = Normalized(s);
        u = Normalized(u);

        Matrix res;
        res << s.x(), s.y(), s.z(), -s.dot(eye), u.x(), u.y(), u.z(),
            -u.dot(eye), -f.x(), -f.y(), -f.z(), f.dot(eye);
        return res;
    }
}

/**
 * @brief 标准化向量
 *
 * @param v 需要标准化的向量
 * @return Vector 标准向量
 */
template <typename Vector, typename Matrix>
Vector Normalized(Vector v) {
    if (std::is_same_v<Matrix, glm::mat4>)
        return glm::normalize(v);
    else
        return v.normalized();
}

/**
 * @brief 返回两个向量叉乘结果
 *
 * @param v1 第一个向量
 * @param v2 第二个向量
 * @return Vector 结果
 */
template <typename Vector, typename Matrix>
Vector Cross(Vector v1, Vector v2) {
    if (std::is_same_v<Vector, glm::vec3>)
        glm::cross(v1, v2);
    else
        return v1.cross(v2);
}

template<typename M>
const float* getMatrixData(const M &m){
    return m.data();
}

template<>
const float* getMatrixData<glm::mat4>(const glm::mat4& m){
    return glm::value_ptr(m);
}
#endif
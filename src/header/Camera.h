#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <Eigen/Eigen>
#include "config.h"
/// @brief 相机移动枚举
enum class DLL_API Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};
// 默认值
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
/**
 * @brief 摄像机类
 * @class
 * @tparam Vector 向量类型
 * @tparam Matrix 矩阵类型
 */
template <typename Vector = glm::vec3, typename Matrix = glm::mat4>
class DLL_API Camera {
  private:
    /**
     * @brief 根据相机的（更新的）欧拉角计算前向量
     *
     */
    void updateCameraVectors();
    // 位置向量
    glm::vec3 Position;
    // 方向向量
    glm::vec3 Front;
    // 上向量
    glm::vec3 Up;
    // 右向量
    glm::vec3 Right;
    // 世界上向量
    glm::vec3 WorldUp;
    // 偏航角
    float Yaw;
    // 俯仰角
    float Pitch;
    // 速度
    float MovementSpeed;
    // DIP
    float MouseSensitivity;
    // 缩放
    float Zoom;

  public:
    /**
     * @brief 基于向量构造Camera
     *
     * @param position 位置向量
     * @param up 世界上向量
     * @param yaw 偏航角 (角度制)
     * @param pitch 俯仰角 (角度制)
     */
    Camera(Vector position = Vector(0.0f, 0.0f, 0.0f),
           Vector up = Vector(0.0f, 1.0f, 0.0f), float yaw = YAW,
           float pitch = PITCH);
    /**
     * @brief 根据标量值来创建Camera
     *
     * @param posX x坐标
     * @param posY y坐标
     * @param posZ z坐标
     * @param upX 世界上向量x
     * @param upY 世界上向量y
     * @param upZ 世界上向量z
     * @param yaw 偏航角 (角度制)
     * @param pitch 俯仰角 (角度制)
     */
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
           float yaw, float pitch);
    /**
     * @brief 返回使用欧拉角和 LookAt 矩阵计算的视图矩阵
     *
     * @return Matrix 视图矩阵
     */
    Matrix GetViewMatrix();
    /**
     * @brief 处理从鼠标滚轮事件接收的输入。只需要在垂直轮轴上输入
     *
     * @param yoffset 垂直滚轮输入值
     */
    void ProcessMouseScroll(float yoffset);
    /**
     * @brief 处理从鼠标输入系统接收的输入。期望 x 和 y 方向上的偏移值
     *
     * @param xoffset x方向的偏移量
     * @param yoffset y方向的偏移量
     * @param constrainPitch 约束间距--确保当间距超出范围时，屏幕不会翻转
     * 默认为ture
     */
    void ProcessMouseMovement(float xoffset, float yoffset,
                              GLboolean constrainPitch = true);
    /**
     * @brief 处理从任何类似键盘的输入系统接收的输入。接受相机定义的 enum
     * 形式的输入参数
     *
     * @param direction 定义的 Camera_Movement 枚举
     * @param deltaTime 当前帧与上一帧的时间差
     */
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    /**
     * @brief 返回相机位置向量
     *
     * @return Vector 位置向量
     */
    inline Vector position() const { return Position; }
    /**
     * @brief 返回相机方向向量
     *
     * @return Vector 方向向量
     */
    inline Vector front() const { return Front; }
    /**
     * @brief 返回相机上向量
     *
     * @return Vector 上向量
     */
    inline Vector up() const { return Up; }
    /**
     * @brief 返回相机右向量
     *
     * @return Vector 右向量
     */
    inline Vector right() const { return Right; }
    /**
     * @brief 返回世界上向量
     *
     * @return Vector 世界上向量
     */
    inline Vector worldUp() const { return WorldUp; }
    /**
     * @brief 返回相机的偏航角
     *
     * @return float 偏航角
     */
    inline float yaw() const { return Yaw; }
    /**
     * @brief 返回相机的俯仰角
     *
     * @return float 俯仰角
     */
    inline float pitch() const { return Pitch; }
    /**
     * @brief 返回相机的移动速度
     *
     * @return float 移动速度
     */
    inline float moveSpeed() const { return MovementSpeed; }
    /**
     * @brief 返回相机的鼠标灵敏度
     *
     * @return float 鼠标灵敏度
     */
    inline float DPI() const { return MouseSensitivity; }
    /**
     * @brief 返回相机的缩放值
     *
     * @return float 缩放值
     */
    inline float zoom() const { return Zoom; }
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
    static Matrix Ortho(float left, float right, float bottom, float top,
                        float zNear, float zFar);
    /**
     * @brief 使用对应的值来获取投影矩阵
     *
     * @param fov 视角大小（弧度）
     * @param aspect 宽高比
     * @param zNear 近平面距离
     * @param zFar 远平面距离
     * @return Matrix 对应的投影矩阵
     */
    static Matrix Perspective(float fov, float aspect, float zNear, float zFar);
    /**
     * @brief 获取LookAt矩阵
     *
     * @param eye 相机位置
     * @param center 目标位置
     * @param up 上向量
     * @return Matrix LookAt矩阵
     */
    static Matrix LookAt(const Vector& eye, const Vector& center,
                         const Vector& up);
    /**
     * @brief 获取对应角度的弧度值
     *
     * @param angle 角度
     * @return float 对应的弧度
     */
    inline static float Radian(float angle) { return angle * PI / 180; }
    /**
     * @brief 标准化向量
     *
     * @param v 需要标准化的向量
     * @return Vector 标准向量
     */
    inline static Vector Normalized(Vector v) { return v.normalized(); }
    /**
     * @brief 标准化向量
     *
     * @param v 需要标准化的向量
     * @return glm::vec3 标准向量
     */
    inline static glm::vec3 Normalized(glm::vec3 v) {
        return glm::normalize(v);
    }
    /**
     * @brief 标准化向量
     *
     * @param v 需要标准化的向量
     * @return Eigen::Vector3f 标准向量
     */
    inline static Eigen::Vector3f Normalized(Eigen::Vector3f v) {
        return v.normalized();
    }
    /**
     * @brief 返回两个向量叉乘结果
     *
     * @param v1 第一个向量
     * @param v2 第二个向量
     * @return Vector 结果
     */
    inline static Vector Cross(Vector v1, Vector v2) { return v1.cross(v2); }
    /**
     * @brief 返回两个向量叉乘结果
     *
     * @param v1 第一个向量
     * @param v2 第二个向量
     * @return glm::vec3 结果
     */
    inline static glm::vec3 Cross(glm::vec3 v1, glm::vec3 v2) {
        return glm::cross(v1, v2);
    }
    /**
     * @brief 返回两个向量叉乘结果
     *
     * @param v1 第一个向量
     * @param v2 第二个向量
     * @return Eigen::Vector3f 结果
     */
    inline static Eigen::Vector3f Cross(Eigen::Vector3f v1,
                                        Eigen::Vector3f v2) {
        return v1.cross(v2);
    }
};
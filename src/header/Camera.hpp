#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <Eigen/Eigen>
#include "defined.h"
#include "template.hpp"
MGL_START
/// @brief 相机移动枚举
enum class Camera_Movement {
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
template <typename Vector, typename Matrix>
class Camera {
  private:
    /**
     * @brief 根据相机的（更新的）欧拉角计算前向量
     *
     */
    void updateCameraVectors() {
        Vector front;
        front.x = cos(Radian(Yaw)) * cos(Radian(Pitch));
        front.y = sin(Radian(Pitch));
        front.z = sin(Radian(Yaw)) * cos(Radian(Pitch));
        Front = Normalized<Vector, Matrix>(front);
        Right = Normalized<Vector, Matrix>(Cross<Vector, Matrix>(Front, WorldUp));
        Up = Normalized<Vector, Matrix>(Cross<Vector, Matrix>(Right, Front));
    }
    // 位置向量
    Vector Position;
    // 方向向量
    Vector Front;
    // 上向量
    Vector Up;
    // 右向量
    Vector Right;
    // 世界上向量
    Vector WorldUp;
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
           float pitch = PITCH)
        : Front(Vector(0.0f, 0.0f, -1.0f)),
          MovementSpeed(SPEED),
          MouseSensitivity(SENSITIVITY),
          Zoom(ZOOM) {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
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
           float yaw, float pitch)
        : Front(Vector(0.0f, 0.0f, -1.0f)),
          MovementSpeed(SPEED),
          MouseSensitivity(SENSITIVITY),
          Zoom(ZOOM) {
        Position = Vector(posX, posY, posZ);
        WorldUp = Vector(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    /**
     * @brief 返回使用欧拉角和 LookAt 矩阵计算的视图矩阵
     *
     * @return Matrix 视图矩阵
     */
    Matrix GetViewMatrix() { return LookAt(Position, Position + Front, Up); }
    /**
     * @brief 处理从鼠标滚轮事件接收的输入。只需要在垂直轮轴上输入
     *
     * @param yoffset 垂直滚轮输入值
     */
    void ProcessMouseScroll(float yoffset) {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f) Zoom = 1.0f;
        if (Zoom > 45.0f) Zoom = 45.0f;
    }

    /**
     * @brief 处理从鼠标输入系统接收的输入。期望 x 和 y 方向上的偏移值
     *
     * @param xoffset x方向的偏移量
     * @param yoffset y方向的偏移量
     * @param constrainPitch 约束间距--确保当间距超出范围时，屏幕不会翻转
     * 默认为ture
     */
    void ProcessMouseMovement(float xoffset, float yoffset,
                              GLboolean constrainPitch = true) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (constrainPitch) {
            if (Pitch > 89.0f) Pitch = 89.0f;
            if (Pitch < -89.0f) Pitch = -89.0f;
        }

        updateCameraVectors();
    }
    /**
     * @brief 处理从任何类似键盘的输入系统接收的输入。接受相机定义的 enum
     * 形式的输入参数
     *
     * @param direction 定义的 Camera_Movement 枚举
     * @param deltaTime 当前帧与上一帧的时间差
     */
    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        switch (direction) {
            case Camera_Movement::FORWARD:
                Position += Front * velocity;
                break;
            case Camera_Movement::BACKWARD:
                Position -= Front * velocity;
                break;
            case Camera_Movement::LEFT:
                Position -= Right * velocity;
                break;
            case Camera_Movement::RIGHT:
                Position += Right * velocity;
                break;

            default:
                break;
        }
    }
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
};

MGL_END
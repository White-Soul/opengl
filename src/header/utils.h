#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Camera.hpp"
#include "stb_image.h"
#include "Template.h"
#define PI 3.1415926

/// @brief 屏幕宽度
extern const int width;
/// @brief 屏幕高度
extern const int height;
/// @brief 当前帧与上一帧的时间差
extern float deltaTime;
/// @brief 上一帧时间
extern float lastFrame;
extern bool firstMouse;
// 摄像机
extern Camera<glm::vec3, glm::mat4> camera;
/**
 * @brief 鼠标回调函数
 *
 * @param window 指向GLFWwindow的指针
 * @param xpos 当前帧的x坐标
 * @param ypos 当前帧的y坐标
 */
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
/**
 * @brief 缩放回调函数
 *
 * @param window 指向GLFWwindow的指针
 * @param xoffset 水平滚动大小
 * @param yoffset 竖直滚动大小
 */
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
/**
 * @deprecated not define
 */
void read_ini(const std::string& path);
/**
 * @deprecated not define
 */
void write_init(const std::string& path);

/**
 * @brief 获取对应角度的弧度值
 *
 * @param angle 角度
 * @return float 对应的弧度
 */
inline static float Radian(float angle) { return angle * PI / 180; }

/**
 * @brief 初始化OpenGL环境
 *
 * @return GLFWwindow* 指向GLWFwindow的指针
 */
GLFWwindow* init();
/**
 * @brief 键盘回调函数
 *
 * @param window 指向GLWFwindow的指针
 */
void processInput(GLFWwindow* window);
/**
 * @brief 读取纹理图片
 *
 * @param path 纹理图片路径
 */
void readImage(const char* path);
/**
 * @brief 载入纹理图片
 *
 * @param path 纹理图片路径
 * @return unsigned int 纹理名称
 */
unsigned int loadTexture(const char* path);
/**
 * @brief 画面调整回调函数
 *
 * @param window 指向GLWFwindow的指针
 * @param width 宽度
 * @param height 高度
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
/**
 * @brief 加载天空盒
 * 顺序:
 * +X (right)
 * -X (left)
 * +Y (top)
 * -Y (bottom)
 * +Z (front)
 * -Z (back)
 * @param faces 包含6个纹理路径的数组
 * @return unsigned int 纹理名称
 */
unsigned int loadCubemap(std::vector<std::string> faces);

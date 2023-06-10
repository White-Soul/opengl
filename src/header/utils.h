#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Camera.h"
#include "stb_image.h"
#include "config.h"
/// @brief 屏幕宽度
const int width = 800;
/// @brief 屏幕高度
const int height = 600;
/// @brief 当前帧与上一帧的时间差
DLL_API extern float deltaTime;
/// @brief 上一帧时间
DLL_API extern float lastFrame;
DLL_API extern bool firstMouse;
// 摄像机
DLL_API extern Camera camera;
/**
 * @brief 鼠标回调函数
 * 
 * @param window 指向GLFWwindow的指针
 * @param xpos 当前帧的x坐标
 * @param ypos 当前帧的y坐标
 */
DLL_API void mouse_callback(GLFWwindow* window, double xpos,
                                       double ypos);
/**
 * @brief 缩放回调函数
 * 
 * @param window 指向GLFWwindow的指针
 * @param xoffset 水平滚动大小
 * @param yoffset 竖直滚动大小
 */
DLL_API void scroll_callback(GLFWwindow* window, double xoffset,
                                        double yoffset);
/**
 * @deprecated not define
*/
DLL_API void read_ini(const std::string& path);
/**
 * @deprecated not define
*/
DLL_API void write_init(const std::string& path);

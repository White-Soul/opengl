#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Camera.h"
#include "stb_image.h"
#include "config.h"

const int width = 800;
const int height = 600;

DLL_API extern float deltaTime;
DLL_API extern float lastFrame;
DLL_API extern bool firstMouse;
DLL_API extern Camera camera;

DLL_API void mouse_callback(GLFWwindow* window, double xpos,
                                       double ypos);

DLL_API void scroll_callback(GLFWwindow* window, double xoffset,
                                        double yoffset);

DLL_API void read_ini(const std::string& path);

DLL_API void write_init(const std::string& path);

#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "config.h"

enum DLL_API Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class DLL_API Camera {
  private:
    void updateCameraVectors();
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

  public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
           float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
           float yaw, float pitch);
    glm::mat4 GetViewMatrix();
    void ProcessMouseScroll(float yoffset);
    void ProcessMouseMovement(float xoffset, float yoffset,
                              GLboolean constrainPitch = true);
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    inline glm::vec3 position() const { return Position; }
    inline glm::vec3 front() const { return Front; }
    inline glm::vec3 up() const { return Up; }
    inline glm::vec3 right() const { return Right; }
    inline glm::vec3 worldUp() const { return WorldUp; }

    inline float yaw() const { return Yaw; }
    inline float pitch() const { return Pitch; }
    inline float moveSpeed() const { return MovementSpeed; }
    inline float DPI() const { return MouseSensitivity; }
    inline float zoom() const { return Zoom; }
};
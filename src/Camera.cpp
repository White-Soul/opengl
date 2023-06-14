#include "header/Camera.h"

template class DLL_API Camera<>;

template <typename Vector, typename Matrix>
void Camera<Vector, Matrix>::updateCameraVectors() {
    Vector front;
    front.x = cos(Radian(Yaw)) * cos(Radian(Pitch));
    front.y = sin(Radian(Pitch));
    front.z = sin(Radian(Yaw)) * cos(Radian(Pitch));
    Front = Normalized(front);
    Right = Normalized(Cross(Front, WorldUp));
    Up = Normalized(Cross(Right, Front));
}

template <typename Vector, typename Matrix>
void Camera<Vector, Matrix>::ProcessMouseScroll(float yoffset) {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f) Zoom = 1.0f;
    if (Zoom > 45.0f) Zoom = 45.0f;
}

template <typename Vector, typename Matrix>
void Camera<Vector, Matrix>::ProcessMouseMovement(float xoffset, float yoffset,
                                                  GLboolean constrainPitch) {
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

template <typename Vector, typename Matrix>
void Camera<Vector, Matrix>::ProcessKeyboard(Camera_Movement direction,
                                             float deltaTime) {
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

template <typename Vector, typename Matrix>
Matrix Camera<Vector, Matrix>::GetViewMatrix() {
    return LookAt(Position, Position + Front, Up);
}

template <typename Vector, typename Matrix>
Camera<Vector, Matrix>::Camera(Vector position, Vector up, float yaw,
                               float pitch)
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

template <typename Vector, typename Matrix>
Camera<Vector, Matrix>::Camera(float posX, float posY, float posZ, float upX,
                               float upY, float upZ, float yaw, float pitch)
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

template <>
glm::mat4 Camera<glm::vec3, glm::mat4>::Ortho(float left, float right,
                                              float bottom, float top,
                                              float zNear, float zFar) {
    return glm::ortho(left, right, bottom, top, zNear, zFar);
}

template <typename Vector, typename Matrix>
Matrix Camera<Vector, Matrix>::Ortho(float left, float right, float bottom,
                                     float top, float zNear, float zFar) {
    Matrix mscale, mtranslate;
    mscale << (2.0 / (right - left)), 0, 0, 0, 0, (2.0 / (top - bottom)), 0, 0,
        0, 0, (2.0 / (zNear - zFar)), 0, 0, 0, 0, 1;
    mtranslate << 1, 0, 0, -(left + right) / 2, 0, 1, 0, -(bottom + top) / 2, 0,
        0, 1, -(zNear + zFar) / 2, 0, 0, 0, 1;
    return mscale * mtranslate;
}

template <>
glm::mat4 Camera<glm::vec3, glm::mat4>::Perspective(float fov, float aspect,
                                                    float zNear, float zFar) {
    return glm::perspective(fov, aspect, zNear, zFar);
}

template <typename Vector, typename Matrix>
Matrix Camera<Vector, Matrix>::Perspective(float fov, float aspect, float zNear,
                                           float zFar) {
    Matrix persp;
    persp << 1 / ((std::tan(fov / 2) * aspect)), 0, 0, 0, 0,
        1 / (std::tan(fov / 2)), 0, 0, 0, 0, (zNear + zFar) / (zNear - zFar),
        -(2 * zNear * zFar) / (zNear - zFar), 0, 0, 1, 0;
}

template <>
glm::mat4 Camera<glm::vec3, glm::mat4>::LookAt(const glm::vec3& eye,
                                               const glm::vec3& center,
                                               const glm::vec3& up) {
    return glm::lookAt(eye, center, up);
}

template <typename Vector, typename Matrix>
Matrix Camera<Vector, Matrix>::LookAt(const Vector& eye, const Vector& center,
                                      const Vector& up) {
    Vector f(center - eye);
    Vector s(f.cross(up));
    Vector u(s.cross(f));
    f = Normalized(f);
    s = Normalized(s);
    u = Normalized(u);

    Matrix res;
    res << s.x(), s.y(), s.z(), -s.dot(eye), u.x(), u.y(), u.z(), -u.dot(eye),
        -f.x(), -f.y(), -f.z(), f.dot(eye);
    return res;
}

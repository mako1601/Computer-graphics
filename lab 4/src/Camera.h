#ifndef _Camera_h_
#define _Camera_h_

#include <glad/glad.h>
#include <glm/glm.hpp>

enum class Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    // camera attributes
    glm::vec3 Position{ 1.3f, 1.7f, 3.0f };
    glm::vec3 Front{ 0.0f, 0.0f, -1.0f };
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp{ 0.0f, 1.0f, 0.0f };
    // euler angles
    float Yaw = -112.0f;
    float Pitch = -30.0f;
    // camera options
    float MovementSpeed = 2.5f;
    float Sensitivity   = 0.15f;
    float Zoom          = 90.0f;

    Camera() { updateCameraVectors(); }

    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
    void updateCameraVectors();
};

#endif
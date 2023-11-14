#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == Movement::FORWARD) {
        Position += Front * velocity;
    }
    if (direction == Movement::BACKWARD) {
        Position -= Front * velocity;
    }
    if (direction == Movement::LEFT) {
        Position -= Right * velocity;
    }
    if (direction == Movement::RIGHT) {
        Position += Right * velocity;
    }
    if (direction == Movement::UP) {
        Position += WorldUp * velocity;
    }
    if (direction == Movement::DOWN) {
        Position -= WorldUp * velocity;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= Sensitivity;
    yoffset *= Sensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (Pitch > 89.999f) {
            Pitch = 89.999f;
        }
        if (Pitch < -89.999f) {
            Pitch = -89.999f;
        }
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= yoffset;
    if (Zoom < 40.0f) {
        Zoom = 40.0f;
    }
    if (Zoom > 140.0f) {
        Zoom = 140.0f;
    }
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // calculate the new Front vector
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}
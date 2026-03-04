#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 startPos)
    : position(startPos), worldUp(0.0f, 1.0f, 0.0f), yaw(-90.0f), pitch(0.0f),
      fov(45.0f), nearPlane(0.1f), farPlane(100.0f) {
    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 f;
    f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    f.y = sin(glm::radians(pitch));
    f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(f);
    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::ProcessKeyboard(int direction, float deltaTime) {
    float velocity = moveSpeed * deltaTime;

    // Create a flattened front vector (XZ plane only)
    glm::vec3 horizontalFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));

    // The right vector is already horizontal if your worldUp is (0, 1, 0),
    // but we can flatten it to be safe.
    glm::vec3 horizontalRight = glm::normalize(glm::vec3(right.x, 0.0f, right.z));

    // WASD movement restricted to X and Z
    if (direction == FORWARD)  position += horizontalFront * velocity;
    if (direction == BACKWARD) position -= horizontalFront * velocity;
    if (direction == LEFT)     position -= horizontalRight * velocity;
    if (direction == RIGHT)    position += horizontalRight * velocity;

    // Vertical movement remains strictly along the world Y axis
    if (direction == UP)       position += worldUp * velocity;
    if (direction == DOWN)     position -= worldUp * velocity;
}

void Camera::ProcessMouse(float xoffset, float yoffset) {
    yaw   += xoffset * sensitivity;
    pitch += yoffset * sensitivity;

    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateCameraVectors();
}

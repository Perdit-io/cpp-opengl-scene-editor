#pragma once

#include <glm/glm.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float moveSpeed = 15.0f;
    float sensitivity = 0.2f;
    float fov;
    float nearPlane;
    float farPlane;

    Camera(glm::vec3 startPos = glm::vec3(0.0f, 1.0f, 8.0f));

    void ProcessKeyboard(int direction, float deltaTime);
    void ProcessMouse(float xoffset, float yoffset);

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(float aspectRatio) const;
private:
    void updateCameraVectors();
};

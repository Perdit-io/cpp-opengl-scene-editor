#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    float fov;
    float nearPlane;
    float farPlane;

    Camera(glm::vec3 startPos = glm::vec3(0.0f, 1.0f, 8.0f));

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(float aspectRatio) const;
};

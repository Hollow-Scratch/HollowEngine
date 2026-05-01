#pragma once

#include <glm/glm.hpp>

namespace Hollow {

class Camera {
public:
    Camera(float fov, float aspect, float nearClip, float farClip);

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;

    void SetPosition(const glm::vec3& position);
    void SetRotation(float yaw, float pitch);
    void SetAspect(float aspect);

    const glm::vec3& GetPosition() const;
    float GetYaw() const;
    float GetPitch() const;

private:
    glm::vec3 GetFront() const;

private:
    glm::vec3 m_Position;

    float m_Yaw;
    float m_Pitch;

    float m_FOV;
    float m_Aspect;
    float m_Near;
    float m_Far;
};

}
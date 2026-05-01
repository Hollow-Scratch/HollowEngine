#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Hollow {

Camera::Camera(float fov, float aspect, float nearClip, float farClip)
    : m_Position(0.0f, 0.0f, 3.0f),
      m_Yaw(-90.0f),
      m_Pitch(0.0f),
      m_FOV(fov),
      m_Aspect(aspect),
      m_Near(nearClip),
      m_Far(farClip)
{
}

glm::mat4 Camera::GetViewMatrix() const
{
    glm::vec3 front = GetFront();
    return glm::lookAt(
        m_Position,
        m_Position + front,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return glm::perspective(
        glm::radians(m_FOV),
        m_Aspect,
        m_Near,
        m_Far
    );
}

void Camera::SetPosition(const glm::vec3& position)
{
    m_Position = position;
}

void Camera::SetRotation(float yaw, float pitch)
{
    // clamp pitch to avoid gimbal flip
    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    m_Yaw = yaw;
    m_Pitch = pitch;
}

void Camera::SetAspect(float aspect)
{
    m_Aspect = aspect;
}

const glm::vec3& Camera::GetPosition() const
{
    return m_Position;
}

float Camera::GetYaw() const
{
    return m_Yaw;
}

float Camera::GetPitch() const
{
    return m_Pitch;
}

glm::vec3 Camera::GetFront() const
{
    glm::vec3 front;

    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

    return glm::normalize(front);
}

}
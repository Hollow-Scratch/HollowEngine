#pragma once

#include "Renderer/Camera.hpp"
#include "glm/glm.hpp"

namespace Hollow {

    struct TransformComponent
    {
        glm::vec3 Position{ 0.0f };
        glm::vec3 Rotation{ 0.0f };
        glm::vec3 Scale{ 1.0f };
    };

    struct MeshComponent
    {
        //glm::vec4 Color{ 1.0f }; plans for future use
    };

    struct CameraComponent
    {
    Camera CameraData;
    bool Primary = true;

    CameraComponent(float fov, float aspect, float nearClip, float farClip)
        : CameraData(fov, aspect, nearClip, farClip) {}
    };

    
}
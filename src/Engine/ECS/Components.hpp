#pragma once

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
}
#pragma once

#include "Renderer/Camera.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/Texture.hpp"
#include <cstdint>
#include <glm/vec3.hpp>
#include <memory>

namespace Hollow {

struct AABBComponent
{
    glm::vec3 Center{0.0f};
    glm::vec3 Extents{0.5f};
};

struct TransformComponent
{
    glm::vec3 Position{0.0f};
    glm::vec3 Rotation{0.0f};
    glm::vec3 Scale{1.0f};
};

struct MeshComponent
{
    std::shared_ptr<VertexArray> VAO;
    std::shared_ptr<VertexBuffer> VBO;
    std::shared_ptr<IndexBuffer> EBO;
    uint32_t IndexCount = 0;
    glm::vec3 LocalBoundsMin{0.5f};
    glm::vec3 LocalBoundsMax{-0.5f};
};

struct MaterialComponent
{
    std::shared_ptr<Texture> Diffuse;
};

struct CameraComponent
{
    Camera CameraData;
    bool Primary = true;

    CameraComponent(float fov, float aspect, float nearClip, float farClip)
        : CameraData(fov, aspect, nearClip, farClip) {}
};

}

#pragma once

#include "Renderer/Camera.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/Texture.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace Hollow {

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

    std::shared_ptr<Texture> TextureData;

    unsigned int IndexCount = 0;
};

struct CameraComponent
{
    Camera CameraData;
    bool Primary = true;

    CameraComponent(float fov, float aspect, float nearClip, float farClip)
        : CameraData(fov, aspect, nearClip, farClip) {}
};

}
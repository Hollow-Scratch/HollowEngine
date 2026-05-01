#pragma once

#include "Renderer/Camera.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/BufferLayout.hpp"

#include <cstdint>
#include <glm/vec3.hpp>
#include <glm/common.hpp>
#include <memory>
#include <vector>

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

    void Setup(const std::vector<float>& vertices,
               const std::vector<uint32_t>& indices)
    {
        VAO = std::make_shared<VertexArray>();
        VBO = std::make_shared<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
        EBO = std::make_shared<IndexBuffer>(indices.data(), indices.size());

        BufferLayout layout;
        layout.PushFloat(3);

        VAO->AddBuffer(*VBO, layout);

        VAO->Bind();
        EBO->Bind();

        IndexCount = indices.size();

        if (!vertices.empty())
        {
            glm::vec3 min(vertices[0], vertices[1], vertices[2]);
            glm::vec3 max = min;

            for (size_t i = 0; i < vertices.size(); i += 3)
            {
                glm::vec3 v(vertices[i], vertices[i + 1], vertices[i + 2]);
                min = glm::min(min, v);
                max = glm::max(max, v);
            }

            LocalBoundsMin = min;
            LocalBoundsMax = max;
        }
    }
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
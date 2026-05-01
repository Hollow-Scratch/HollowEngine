#include "Renderer.hpp"
#include "Graphics/Shader.hpp"
#include "Renderer/Framebuffer.hpp"
#include "Renderer/Frustum.hpp"
#include "Renderer/Camera.hpp"
#include "ECS/Registry.hpp"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstddef>
#include <vector>

namespace Hollow {

namespace {

struct InstanceBatch
{
    const MeshComponent* Mesh = nullptr;
    std::vector<glm::mat4> Models;
};

bool IsSameBatch(const InstanceBatch& batch, const MeshComponent& mesh)
{
    return batch.Mesh != nullptr &&
           batch.Mesh->VAO.get() == mesh.VAO.get() &&
           batch.Mesh->EBO.get() == mesh.EBO.get() &&
           batch.Mesh->TextureData.get() == mesh.TextureData.get() &&
           batch.Mesh->IndexCount == mesh.IndexCount;
}

InstanceBatch& GetOrCreateBatch(std::vector<InstanceBatch>& batches, const MeshComponent& mesh, std::size_t& activeBatchCount)
{
    for (std::size_t i = 0; i < activeBatchCount; ++i)
    {
        if (IsSameBatch(batches[i], mesh))
            return batches[i];
    }

    if (activeBatchCount == batches.size())
        batches.emplace_back();

    InstanceBatch& batch = batches[activeBatchCount++];
    batch.Mesh = &mesh;
    batch.Models.clear();
    return batch;
}

void SetupInstanceAttributes()
{
    for (unsigned int i = 0; i < 4; ++i)
    {
        glEnableVertexAttribArray(3 + i);
        glVertexAttribPointer(
            3 + i,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::mat4),
            reinterpret_cast<void*>(sizeof(float) * 4 * i)
        );
        glVertexAttribDivisor(3 + i, 1);
    }
}

void UploadInstanceData(std::size_t byteSize, const glm::mat4* models, std::size_t& capacity)
{
    if (byteSize > capacity)
    {
        glBufferData(GL_ARRAY_BUFFER, byteSize, models, GL_DYNAMIC_DRAW);
        capacity = byteSize;
        return;
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, byteSize, models);
}

}

std::unique_ptr<Shader> Renderer::s_Shader;
std::unique_ptr<Framebuffer> Renderer::s_Framebuffer;
static unsigned int s_InstanceVBO = 0;
static std::size_t s_InstanceBufferCapacity = 0;
static Frustum s_Frustum;
static std::vector<InstanceBatch> s_InstanceBatches;
static glm::vec4 s_ClearColor(0.1f, 0.1f, 0.2f, 1.0f);

void Renderer::Init()
{
    glEnable(GL_DEPTH_TEST);

    s_Shader = std::make_unique<Shader>(
        "assets/shaders/basic.vert",
        "assets/shaders/basic.frag"
    );

    s_Framebuffer = std::make_unique<Framebuffer>(1, 1);
    glGenBuffers(1, &s_InstanceVBO);
}

void Renderer::Shutdown()
{
    if (s_InstanceVBO)
    {
        glDeleteBuffers(1, &s_InstanceVBO);
        s_InstanceVBO = 0;
        s_InstanceBufferCapacity = 0;
    }

    if (s_Shader)
        s_Shader->Destroy();

    s_InstanceBatches.clear();
    s_Framebuffer.reset();
    s_Shader.reset();
}

void Renderer::Clear(float r, float g, float b)
{
    s_ClearColor = glm::vec4(r, g, b, 1.0f);
}

void Renderer::Resize(uint32_t width, uint32_t height)
{
    if (!s_Framebuffer || width == 0 || height == 0)
        return;

    s_Framebuffer->Resize(width, height);
}

void Renderer::Draw(Registry& registry, float width, float height)
{
    if (width <= 0.0f || height <= 0.0f || !s_Framebuffer)
        return;

    const auto framebufferWidth = static_cast<uint32_t>(width);
    const auto framebufferHeight = static_cast<uint32_t>(height);

    Resize(framebufferWidth, framebufferHeight);
    s_Framebuffer->Bind();
    glViewport(0, 0, static_cast<GLsizei>(framebufferWidth), static_cast<GLsizei>(framebufferHeight));
    glEnable(GL_DEPTH_TEST);
    glClearColor(s_ClearColor.r, s_ClearColor.g, s_ClearColor.b, s_ClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    Camera* activeCamera = nullptr;

    for (auto& [entity, cam] : registry.GetCameras())
    {
        if (cam.Primary)
        {
            activeCamera = &cam.CameraData;
            break;
        }
    }

    if (!activeCamera)
    {
        s_Framebuffer->Unbind();
        return;
    }

    if (height > 0.0f)
        activeCamera->SetAspect(width / height);

    s_Shader->Bind();
    s_Shader->SetInt("u_Texture", 0);

    const glm::mat4 vp =
        activeCamera->GetProjectionMatrix() *
        activeCamera->GetViewMatrix();
    s_Frustum.Update(vp);

    auto& meshes = registry.GetMeshes();
    auto& transforms = registry.GetTransforms();
    auto& aabbs = registry.GetAABBs();

    if (meshes.empty())
    {
        s_Framebuffer->Unbind();
        return;
    }

    std::size_t activeBatchCount = 0;

    for (auto& [entity, mesh] : meshes)
    {
        auto transformIt = transforms.find(entity);
        if (transformIt == transforms.end())
            continue;

        if (!mesh.VAO || !mesh.EBO)
            continue;

        auto aabbIt = aabbs.find(entity);
        if (aabbIt == aabbs.end())
            continue;

        auto& transform = transformIt->second;
        auto& aabb = aabbIt->second;

        aabb.Center = transform.Position;
        aabb.Extents = glm::abs(transform.Scale) * 0.5f;

        if (!s_Frustum.Intersects(aabb))
            continue;

        glm::mat4 model(1.0f);
        model = glm::translate(model, transform.Position);

        if (transform.Rotation.x != 0.0f)
            model = glm::rotate(model, glm::radians(transform.Rotation.x), {1,0,0});
        if (transform.Rotation.y != 0.0f)
            model = glm::rotate(model, glm::radians(transform.Rotation.y), {0,1,0});
        if (transform.Rotation.z != 0.0f)
            model = glm::rotate(model, glm::radians(transform.Rotation.z), {0,0,1});

        model = glm::scale(model, transform.Scale);

        InstanceBatch& batch = GetOrCreateBatch(s_InstanceBatches, mesh, activeBatchCount);
        batch.Models.push_back(model);
    }

    if (activeBatchCount == 0)
    {
        s_Framebuffer->Unbind();
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, s_InstanceVBO);
    s_Shader->SetMat4("u_ViewProjection", glm::value_ptr(vp));

    for (std::size_t i = 0; i < activeBatchCount; ++i)
    {
        const InstanceBatch& batch = s_InstanceBatches[i];
        const std::size_t batchByteSize = batch.Models.size() * sizeof(glm::mat4);

        UploadInstanceData(batchByteSize, batch.Models.data(), s_InstanceBufferCapacity);

        batch.Mesh->VAO->Bind();
        batch.Mesh->EBO->Bind();
        SetupInstanceAttributes();

        if (batch.Mesh->TextureData)
            batch.Mesh->TextureData->Bind(0);

        glDrawElementsInstanced(
            GL_TRIANGLES,
            batch.Mesh->IndexCount,
            GL_UNSIGNED_INT,
            nullptr,
            static_cast<GLsizei>(batch.Models.size())
        );
    }

    s_Framebuffer->Unbind();
}

uint32_t Renderer::GetFinalImage()
{
    if (!s_Framebuffer)
        return 0;

    return s_Framebuffer->GetColorAttachmentID();
}

}

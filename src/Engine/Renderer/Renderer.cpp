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
#include <vector>

namespace Hollow {

namespace {

struct InstanceBatch
{
    const MeshComponent* Mesh = nullptr;
    const MaterialComponent* Material = nullptr;
    std::vector<glm::mat4> Models;
};

bool IsSameBatch(const InstanceBatch& batch,
                 const MeshComponent& mesh,
                 const MaterialComponent* material)
{
    const Texture* batchTexture = batch.Material ? batch.Material->Diffuse.get() : nullptr;
    const Texture* materialTexture = material ? material->Diffuse.get() : nullptr;

    return batch.Mesh &&
           batch.Mesh->VAO.get() == mesh.VAO.get() &&
           batch.Mesh->EBO.get() == mesh.EBO.get() &&
           batchTexture == materialTexture &&
           batch.Mesh->IndexCount == mesh.IndexCount;
}

InstanceBatch& GetOrCreateBatch(std::vector<InstanceBatch>& batches,
                                const MeshComponent& mesh,
                                const MaterialComponent* material,
                                size_t& activeCount)
{
    for (size_t i = 0; i < activeCount; i++)
    {
        if (IsSameBatch(batches[i], mesh, material))
            return batches[i];
    }

    if (activeCount == batches.size())
        batches.emplace_back();

    InstanceBatch& batch = batches[activeCount++];
    batch.Mesh = &mesh;
    batch.Material = material;
    batch.Models.clear();
    return batch;
}

void SetupInstanceAttributes()
{
    for (int i = 0; i < 4; i++)
    {
        glEnableVertexAttribArray(3 + i);
        glVertexAttribPointer(
            3 + i,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::mat4),
            (void*)(sizeof(float) * 4 * i)
        );
        glVertexAttribDivisor(3 + i, 1);
    }
}

void UploadInstanceData(size_t size, const glm::mat4* data, size_t& capacity)
{
    if (size > capacity)
    {
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
        capacity = size;
    }
    else
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
}

}

std::unique_ptr<Shader> Renderer::s_Shader;
std::unique_ptr<Framebuffer> Renderer::s_Framebuffer;

static unsigned int s_InstanceVBO = 0;
static size_t s_InstanceCapacity = 0;
static Frustum s_Frustum;
static std::vector<InstanceBatch> s_Batches;
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
        s_InstanceCapacity = 0;
    }

    if (s_Shader)
        s_Shader->Destroy();

    s_Batches.clear();

    s_Framebuffer.reset();
    s_Shader.reset();
}

void Renderer::Clear(float r, float g, float b)
{
    s_ClearColor = { r, g, b, 1.0f };
}

void Renderer::Resize(uint32_t w, uint32_t h)
{
    if (!s_Framebuffer || w == 0 || h == 0)
        return;

    s_Framebuffer->Resize(w, h);
}

void Renderer::Draw(Registry& registry, float width, float height)
{
    if (width <= 0 || height <= 0)
        return;

    uint32_t w = (uint32_t)width;
    uint32_t h = (uint32_t)height;

    if (s_Framebuffer)
        Resize(w, h);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, w, h);
    glClearColor(s_ClearColor.r, s_ClearColor.g, s_ClearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Camera* cam = nullptr;

    for (auto& [e, c] : registry.GetCameras())
    {
        if (c.Primary)
        {
            cam = &c.CameraData;
            break;
        }
    }

    if (!cam)
        return;

    cam->SetAspect(width / height);

    s_Shader->Bind();
    s_Shader->SetInt("u_Texture", 0);

    glm::mat4 vp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
    s_Frustum.Update(vp);

    auto& meshes = registry.GetMeshes();
    auto& materials = registry.GetMaterials();
    auto& transforms = registry.GetTransforms();
    auto& aabbs = registry.GetAABBs();

    size_t activeBatchCount = 0;

    for (auto& [entity, mesh] : meshes)
    {
        auto tIt = transforms.find(entity);
        if (tIt == transforms.end())
            continue;

        if (!mesh.VAO || !mesh.EBO)
            continue;

        auto aIt = aabbs.find(entity);
        if (aIt == aabbs.end())
            continue;

        auto& transform = tIt->second;
        auto& aabb = aIt->second;
        const MaterialComponent* material = nullptr;

        auto mIt = materials.find(entity);
        if (mIt != materials.end())
            material = &mIt->second;

        glm::mat4 model(1.0f);
        model = glm::translate(model, transform.Position);

        if (transform.Rotation.x)
            model = glm::rotate(model, glm::radians(transform.Rotation.x), {1,0,0});
        if (transform.Rotation.y)
            model = glm::rotate(model, glm::radians(transform.Rotation.y), {0,1,0});
        if (transform.Rotation.z)
            model = glm::rotate(model, glm::radians(transform.Rotation.z), {0,0,1});

        model = glm::scale(model, transform.Scale);

        const bool hasValidLocalBounds =
            mesh.LocalBoundsMax.x >= mesh.LocalBoundsMin.x &&
            mesh.LocalBoundsMax.y >= mesh.LocalBoundsMin.y &&
            mesh.LocalBoundsMax.z >= mesh.LocalBoundsMin.z;

        if (hasValidLocalBounds)
        {
            const glm::vec3 localCenter = (mesh.LocalBoundsMin + mesh.LocalBoundsMax) * 0.5f;
            const glm::vec3 localExtents = (mesh.LocalBoundsMax - mesh.LocalBoundsMin) * 0.5f;
            const glm::mat3 linearTransform(model);

            glm::mat3 absLinearTransform(1.0f);
            absLinearTransform[0] = glm::abs(linearTransform[0]);
            absLinearTransform[1] = glm::abs(linearTransform[1]);
            absLinearTransform[2] = glm::abs(linearTransform[2]);

            aabb.Center = glm::vec3(model * glm::vec4(localCenter, 1.0f));
            aabb.Extents = absLinearTransform * localExtents;

            if (!s_Frustum.Intersects(aabb))
                continue;
        }

        InstanceBatch& batch = GetOrCreateBatch(s_Batches, mesh, material, activeBatchCount);
        batch.Models.push_back(model);
    }

    if (activeBatchCount == 0)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, s_InstanceVBO);
    s_Shader->SetMat4("u_ViewProjection", glm::value_ptr(vp));

    for (size_t i = 0; i < activeBatchCount; i++)
    {
        const InstanceBatch& batch = s_Batches[i];
        size_t size = batch.Models.size() * sizeof(glm::mat4);

        UploadInstanceData(size, batch.Models.data(), s_InstanceCapacity);

        batch.Mesh->VAO->Bind();
        batch.Mesh->EBO->Bind();
        SetupInstanceAttributes();

        if (batch.Material && batch.Material->Diffuse)
            batch.Material->Diffuse->Bind(0);
        else
            glBindTexture(GL_TEXTURE_2D, 0);

        glDrawElementsInstanced(
            GL_TRIANGLES,
            batch.Mesh->IndexCount,
            GL_UNSIGNED_INT,
            nullptr,
            (GLsizei)batch.Models.size()
        );
    }
}

uint32_t Renderer::GetFinalImage()
{
    if (!s_Framebuffer)
        return 0;

    return s_Framebuffer->GetColorAttachmentID();
}

}

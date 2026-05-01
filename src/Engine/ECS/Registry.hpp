#pragma once

#include <unordered_map>
#include "Entity.hpp"
#include "Components.hpp"

namespace Hollow {

class Registry
{
public:
    Entity CreateEntity()
    {
        return m_NextEntity++;
    }

    void DestroyEntity(Entity entity)
    {
        m_Transforms.erase(entity);
        m_Meshes.erase(entity);
        m_Cameras.erase(entity);
    }

    TransformComponent& AddTransform(Entity entity)
    {
        return m_Transforms[entity];
    }

    MeshComponent& AddMesh(Entity entity)
    {
        return m_Meshes[entity];
    }

    CameraComponent& AddCamera(Entity entity, float fov, float aspect, float nearClip, float farClip)
    {
        return m_Cameras.emplace(entity, CameraComponent(fov, aspect, nearClip, farClip)).first->second;
    }

    std::unordered_map<Entity, TransformComponent>& GetTransforms()
    {
        return m_Transforms;
    }

    std::unordered_map<Entity, MeshComponent>& GetMeshes()
    {
        return m_Meshes;
    }

    std::unordered_map<Entity, CameraComponent>& GetCameras()
    {
        return m_Cameras;
    }

private:
    Entity m_NextEntity = 1;

    std::unordered_map<Entity, TransformComponent> m_Transforms;
    std::unordered_map<Entity, MeshComponent> m_Meshes;
    std::unordered_map<Entity, CameraComponent> m_Cameras;
};

}
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
    }

    // Add components
    TransformComponent& AddTransform(Entity entity)
    {
        return m_Transforms[entity];
    }

    MeshComponent& AddMesh(Entity entity)
    {
        return m_Meshes[entity];
    }

    // Access
    std::unordered_map<Entity, TransformComponent>& GetTransforms()
    {
        return m_Transforms;
    }

    std::unordered_map<Entity, MeshComponent>& GetMeshes()
    {
        return m_Meshes;
    }

private:
    Entity m_NextEntity = 1;

    std::unordered_map<Entity, TransformComponent> m_Transforms;
    std::unordered_map<Entity, MeshComponent> m_Meshes;
};

}
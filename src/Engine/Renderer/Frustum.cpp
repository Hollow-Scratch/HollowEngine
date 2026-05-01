#include "Frustum.hpp"

#include "ECS/Components.hpp"

#include <glm/common.hpp>
#include <glm/geometric.hpp>

namespace Hollow {

void Frustum::Update(const glm::mat4& viewProjection)
{
    const glm::vec4 row0(
        viewProjection[0][0],
        viewProjection[1][0],
        viewProjection[2][0],
        viewProjection[3][0]
    );
    const glm::vec4 row1(
        viewProjection[0][1],
        viewProjection[1][1],
        viewProjection[2][1],
        viewProjection[3][1]
    );
    const glm::vec4 row2(
        viewProjection[0][2],
        viewProjection[1][2],
        viewProjection[2][2],
        viewProjection[3][2]
    );
    const glm::vec4 row3(
        viewProjection[0][3],
        viewProjection[1][3],
        viewProjection[2][3],
        viewProjection[3][3]
    );

    m_Planes[0] = BuildPlane(row3 + row0);
    m_Planes[1] = BuildPlane(row3 - row0);
    m_Planes[2] = BuildPlane(row3 + row1);
    m_Planes[3] = BuildPlane(row3 - row1);
    m_Planes[4] = BuildPlane(row3 + row2);
    m_Planes[5] = BuildPlane(row3 - row2);
}

bool Frustum::Intersects(const AABBComponent& aabb) const
{
    for (const Plane& plane : m_Planes)
    {
        const glm::vec3 absNormal = glm::abs(plane.Normal);
        const float radius = glm::dot(aabb.Extents, absNormal);
        const float distance = glm::dot(plane.Normal, aabb.Center) + plane.D;

        if (distance + radius < 0.0f)
            return false;
    }

    return true;
}

Frustum::Plane Frustum::BuildPlane(const glm::vec4& coefficients)
{
    Plane plane;
    plane.Normal = glm::vec3(coefficients);

    const float length = glm::length(plane.Normal);
    if (length > 0.0f)
    {
        const float inverseLength = 1.0f / length;
        plane.Normal *= inverseLength;
        plane.D = coefficients.w * inverseLength;
    }

    return plane;
}

}

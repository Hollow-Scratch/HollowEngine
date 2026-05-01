#pragma once

#include <array>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Hollow {

struct AABBComponent;

class Frustum
{
public:
    struct Plane
    {
        glm::vec3 Normal{0.0f};
        float D = 0.0f;
    };

    void Update(const glm::mat4& viewProjection);
    bool Intersects(const AABBComponent& aabb) const;

private:
    static Plane BuildPlane(const glm::vec4& coefficients);

private:
    std::array<Plane, 6> m_Planes{};
};

}

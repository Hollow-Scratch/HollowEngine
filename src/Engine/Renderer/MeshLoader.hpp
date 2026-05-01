#pragma once
#include "ECS/Components.hpp"
#include <string>

namespace Hollow {

class MeshLoader {
public:
    static MeshComponent LoadFromFile(const std::string& path);
};

}
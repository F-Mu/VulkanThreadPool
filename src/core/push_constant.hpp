#pragma once
#include <glm/glm.hpp>
namespace crp{
    struct SimplePushConstantData {
        glm::mat4 modelMatrix{1.f};
        glm::mat4 normalMatrix{1.f};
    };
}
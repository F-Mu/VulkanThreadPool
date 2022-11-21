#pragma once

#include "crp_camera.hpp"
#include "resources/game_object_manager.hpp"

#include <vulkan/vulkan.h>

namespace crp {
#define MAX_LIGHTS 10
    struct PointLight {
        glm::vec4 position{};
        glm::vec4 color{};
    };

    struct GlobalUbo {
        alignas(16) glm::mat4 projection{1.f};
        alignas(16) glm::mat4 view{1.f};
        alignas(16) glm::mat4 inverseView{1.f};
        alignas(16) glm::vec4 ambientLightColor{1.f, 1.f, 1.f, .02f};
        PointLight pointLights[MAX_LIGHTS];
        int numLights;
    };

    struct FrameInfo {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        CrpCamera &camera;
        VkDescriptorSet globalDescriptorSet;
        GameObjectManager::Map &gameObjects;
    };
}
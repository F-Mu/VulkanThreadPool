#pragma once

#include "render_camera.hpp"
#include "resources/manager/game_object_manager.hpp"

#include <vulkan/vulkan.h>

namespace crp {

    struct GlobalUbo {
        alignas(16) glm::mat4 projection{1.f};
        alignas(16) glm::mat4 view{1.f};
        alignas(16) glm::mat4 inverseView{1.f};
    };

    struct RenderFrameInfo {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        VkDescriptorSet globalDescriptorSet;
        GameObjectManager::Map &gameObjects;
    };
}
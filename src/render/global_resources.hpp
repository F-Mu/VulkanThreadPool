#pragma once

#include <vulkan/vulkan.h>
#include "render_descriptors.hpp"
#include "render_swap_chain.hpp"
#include "render_buffer.hpp"
#include "render_frame_info.hpp"
namespace crp {
    class GlobalResources {
    public:
        GlobalResources(RenderDevice &device);

        void clear();
        std::unique_ptr<RenderDescriptorPool> globalPool{};
        std::unique_ptr<RenderDescriptorSetLayout> globalSetLayout{};
        std::vector<std::unique_ptr<RenderBuffer>> uboBuffers;
        std::vector<VkDescriptorSet> globalDescriptorSets;
    };
}
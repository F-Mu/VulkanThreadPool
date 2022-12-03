#pragma once

#include <vulkan/vulkan.h>
#include "crp_descriptors.hpp"
#include "crp_swap_chain.hpp"
#include "crp_buffer.hpp"
#include "crp_frame_info.hpp"
namespace crp {
    class GlobalResources {
    public:
        GlobalResources(CrpDevice &device);

        void clear();
        std::unique_ptr<CrpDescriptorPool> globalPool{};
        std::unique_ptr<CrpDescriptorSetLayout> globalSetLayout{};
        std::vector<std::unique_ptr<CrpBuffer>> uboBuffers;
        std::vector<VkDescriptorSet> globalDescriptorSets;
    };
}
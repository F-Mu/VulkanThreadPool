#pragma once

#include "render/crp_camera.hpp"
#include "render/crp_pipeline.hpp"
#include "render/crp_device.hpp"
#include "function/framework/crp_game_obejct.hpp"
#include "render/crp_frame_info.hpp"
//std
#include <memory>
#include <vector>

namespace crp {
    class SimpleRenderPass {
    public:
        SimpleRenderPass(CrpDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);

        ~SimpleRenderPass();

        SimpleRenderPass(const SimpleRenderPass &) = delete;

        SimpleRenderPass &operator=(const SimpleRenderPass &) = delete;

        void tick(FrameInfo &frameInfo);

//    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);

        void createPipeline(VkRenderPass renderPass);

        CrpDevice &crpDevice;
        std::unique_ptr<CrpPipeline> crpPipeline;
        VkPipelineLayout pipelineLayout;
    };
}
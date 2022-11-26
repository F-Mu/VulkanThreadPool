#pragma once

#include "render/crp_camera.hpp"
#include "render/crp_pipeline.hpp"
#include "render/crp_device.hpp"
#include "render/crp_game_obejct.hpp"
#include "render/crp_frame_info.hpp"
//std
#include <memory>
#include <vector>

namespace crp {
    class SimpleRenderSystem {
    public:
        SimpleRenderSystem(CrpDevice &device, VkRenderPass renderPass,VkDescriptorSetLayout globalSetLayout);

        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem &) = delete;

        SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

        void renderGameObjects(FrameInfo &frameInfo);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);

        void createPipeline(VkRenderPass renderPass);

        CrpDevice &crpDevice;
        std::unique_ptr<CrpPipeline> crpPipeline;
        VkPipelineLayout pipelineLayout;
    };
}
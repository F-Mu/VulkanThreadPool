#pragma once

#include "../crp_camera.hpp"
#include "../crp_pipeline.hpp"
#include "../crp_device.hpp"
#include "../crp_game_obejct.hpp"
#include "../crp_frame_info.hpp"
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
#pragma once

#include "render/render_camera.hpp"
#include "render/render_pipeline.hpp"
#include "render/render_device.hpp"
#include "function/framework/game_object.hpp"
#include "render/render_frame_info.hpp"
//std
#include <memory>
#include <vector>

namespace crp {
    class SimpleRenderPass {
    public:
        SimpleRenderPass(RenderDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);

        ~SimpleRenderPass();

        SimpleRenderPass(const SimpleRenderPass &) = delete;

        SimpleRenderPass &operator=(const SimpleRenderPass &) = delete;

        void tick(RenderFrameInfo &frameInfo);

//    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);

        void createPipeline(VkRenderPass renderPass);

        RenderDevice &renderDevice;
        std::unique_ptr<RenderPipeline> renderPipeline;
        VkPipelineLayout pipelineLayout;
    };
}
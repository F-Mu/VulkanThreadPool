#pragma once

#include "render/render_camera.hpp"
#include "render/render_pipeline.hpp"
#include "render/render_device.hpp"
#include "function/framework/game_object.hpp"
#include "render/render_frame_info.hpp"
#include "render/render_model.hpp"
//std
#include <memory>
#include <vector>

namespace crp {
    struct PushConstantData {
        glm::mat4 modelMatrix{1.f};
        glm::mat4 normalMatrix{1.f};
    };

    class SimpleRenderPass {
    public:
        SimpleRenderPass(RenderDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);

        ~SimpleRenderPass();

        SimpleRenderPass(const SimpleRenderPass &) = delete;

        SimpleRenderPass &operator=(const SimpleRenderPass &) = delete;

        void tick();

        void bind(PushConstantData &pushConstantData, std::shared_ptr<Model> &model);

//    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);

        void createPipeline(VkRenderPass renderPass);

        RenderDevice &renderDevice;
        std::unique_ptr<RenderPipeline> renderPipeline;
        VkPipelineLayout pipelineLayout;
        std::list<std::pair<PushConstantData, std::shared_ptr<Model>>> data;
    };
}
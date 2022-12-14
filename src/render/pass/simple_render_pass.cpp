#include "simple_render_pass.hpp"

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
//std
#include <stdexcept>
#include <array>
#include <cassert>
#include "function/framework/component/render_component.hpp"
#include "function/global/global_context.hpp"

namespace crp {
    SimpleRenderPass::SimpleRenderPass(RenderDevice &device, VkRenderPass renderPass,
                                       VkDescriptorSetLayout globalSetLayout) : renderDevice{device} {
        createPipelineLayout(globalSetLayout);
        createPipeline(renderPass);
    }

    SimpleRenderPass::~SimpleRenderPass() {
        vkDestroyPipelineLayout(renderDevice.device(), pipelineLayout, nullptr);
    }

    void SimpleRenderPass::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PushConstantData);

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t >(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
//        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if (vkCreatePipelineLayout(renderDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout");
        }
    }

    void SimpleRenderPass::createPipeline(VkRenderPass renderPass) {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline Layout");

        PipelineConfigInfo pipelineConfig{};
        RenderPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        renderPipeline = std::make_unique<RenderPipeline>(
                renderDevice,
                "shaders/simple_shader.vert.spv",
                "shaders/simple_shader.frag.spv",
                pipelineConfig
        );
    }

    void SimpleRenderPass::bind(PushConstantData &pushConstantData, std::shared_ptr<Model> &model) {
        data.emplace_back(pushConstantData, model);
    }

    void SimpleRenderPass::tick() {
        renderPipeline->bind(*globalContext.renderSystem->nowCommandBuffer);

        vkCmdBindDescriptorSets(
                *globalContext.renderSystem->nowCommandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                pipelineLayout,
                0, 1,
                &globalContext.globalResources->globalDescriptorSets[globalContext.renderSystem->getFrameIndex()],
                0, nullptr);
        for (auto &[pushData, model]: data) {
            vkCmdPushConstants(
                    *globalContext.renderSystem->nowCommandBuffer,
                    globalContext.simpleRenderPass->pipelineLayout,
                    VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                    0,
                    sizeof(PushConstantData),
                    &pushData
            );
            model->bind(*globalContext.renderSystem->nowCommandBuffer);
            model->draw(*globalContext.renderSystem->nowCommandBuffer);
        }
        data.clear();
    }
}
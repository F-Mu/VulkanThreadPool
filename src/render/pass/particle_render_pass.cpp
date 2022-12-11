#include "particle_render_pass.hpp"
#include "function/global/global_context.hpp"

namespace crp {
    ParticleRenderPass::ParticleRenderPass(RenderDevice &device, VkRenderPass renderPass,
                                           VkDescriptorSetLayout globalSetLayout) : renderDevice{device} {
        createPipelineLayout(globalSetLayout);
        createPipeline(renderPass);
    }

    ParticleRenderPass::~ParticleRenderPass() {
        vkDestroyPipelineLayout(renderDevice.device(), pipelineLayout, nullptr);
    }

    void ParticleRenderPass::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(ParticlePushConstants);

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

    void ParticleRenderPass::createPipeline(VkRenderPass renderPass) {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline Layout");

        PipelineConfigInfo pipelineConfig{};
        RenderPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.bindingDescriptions.clear();
        pipelineConfig.attributeDescriptions.clear();
        pipelineConfig.pipelineLayout = pipelineLayout;
        renderPipeline = std::make_unique<RenderPipeline>(
                renderDevice,
                "shaders/particle_shader.vert.spv",
                "shaders/particle_shader.frag.spv",
                pipelineConfig
        );
    }

    void ParticleRenderPass::tick() {
        renderPipeline->bind(*globalContext.renderSystem->nowCommandBuffer);

        vkCmdBindDescriptorSets(
                *globalContext.renderSystem->nowCommandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                pipelineLayout,
                0, 1,
                &globalContext.globalResources->globalDescriptorSets[globalContext.renderSystem->getFrameIndex()],
                0, nullptr);
        for (auto &i: resources) {
            vkCmdPushConstants(
                    *globalContext.renderSystem->nowCommandBuffer,
                    globalContext.particleRenderPass->pipelineLayout,
                    VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                    0,
                    sizeof(ParticlePushConstants),
                    &i
            );
            vkCmdDraw(*globalContext.renderSystem->nowCommandBuffer, 6, 1, 0, 0);
        }
        resources.clear();
    }

    void ParticleRenderPass::bind(const ParticlePushConstants &con) {
        resources.emplace_back(con);
    }
}
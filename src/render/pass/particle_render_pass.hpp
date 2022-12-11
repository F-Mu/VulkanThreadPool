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
    struct ParticlePushConstants {
        glm::vec4 position{};
        glm::vec4 color{};
    };

    class ParticleRenderPass {
    public:
        ParticleRenderPass(RenderDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);

        ~ParticleRenderPass();

        ParticleRenderPass(const ParticleRenderPass &) = delete;

        ParticleRenderPass &operator=(const ParticleRenderPass &) = delete;

        void tick();

        void bind(const ParticlePushConstants &con);

//    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);

        void createPipeline(VkRenderPass renderPass);

        RenderDevice &renderDevice;
        std::unique_ptr<RenderPipeline> renderPipeline;
        VkPipelineLayout pipelineLayout;
        std::list<ParticlePushConstants> resources;
    };
}
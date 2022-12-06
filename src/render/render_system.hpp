#pragma once

#include "window_system.hpp"
#include "render_device.hpp"
#include "render_swap_chain.hpp"
//std
#include <cassert>
#include <memory>
#include <vector>

namespace crp {
    class RenderSystem {
    public:
        RenderSystem(WindowSystem &window, RenderDevice &device);

        ~RenderSystem();

        RenderSystem(const RenderSystem &) = delete;

        RenderSystem &operator=(const RenderSystem &) = delete;

        VkRenderPass getSwapChainRenderPass() const { return renderSwapChain->getRenderPass(); }

        float getAspectRatio() const { return renderSwapChain->extentAspectRatio(); }

        bool isFrameInProgress() const {
            return isFrameStarted;
        }

        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const {
            assert(isFrameStarted && "Cannot get frame index when frame not in progress");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();

        void endFrame();

        void beginSwapChainRenderPass();

        void endSwapChainRenderPass();

        std::shared_ptr<VkCommandBuffer> nowCommandBuffer;

    private:
        void createCommandBuffers();

        void freeCommandBuffers();

        void recreateSwapChain();

        WindowSystem &windowSystem;
        RenderDevice &renderDevice;
        std::unique_ptr<RenderSwapChain> renderSwapChain{};
        std::vector<VkCommandBuffer> commandBuffers{};

        uint32_t currentImageIndex{};
        int currentFrameIndex{};
        bool isFrameStarted{};
    };
}
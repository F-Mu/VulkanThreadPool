#pragma once

#include "crp_window.hpp"
#include "crp_device.hpp"
#include "crp_swap_chain.hpp"
//std
#include <cassert>
#include <memory>
#include <vector>

namespace crp {
    class CrpRenderer {
    public:

        CrpRenderer(CrpWindow &window, CrpDevice &device);

        ~CrpRenderer();

        CrpRenderer(const CrpRenderer &) = delete;

        CrpRenderer &operator=(const CrpRenderer &) = delete;

        VkRenderPass getSwapChainRenderPass() const { return crpSwapChain->getRenderPass(); }

        float getAspectRatio() const { return crpSwapChain->extentAspectRatio(); }

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

        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);

        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:
        void createCommandBuffers();

        void freeCommandBuffers();

        void recreateSwapChain();

        CrpWindow &crpWindow;
        CrpDevice &crpDevice;
        std::unique_ptr<CrpSwapChain> crpSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex;
        bool isFrameStarted;
    };
}
#include "crp_renderer.hpp"


//std
#include <stdexcept>
#include <array>
#include <cassert>
#include <iostream>

namespace crp {


    CrpRenderer::CrpRenderer(CrpWindow &window, CrpDevice &device) : crpWindow{window}, crpDevice{device} {
        recreateSwapChain();
        createCommandBuffers();
    }

    CrpRenderer::~CrpRenderer() {
        freeCommandBuffers();
    }

    void CrpRenderer::freeCommandBuffers() {
        vkFreeCommandBuffers(
                crpDevice.device(),
                crpDevice.getCommandPool(),
                static_cast<uint32_t>(commandBuffers.size()),
                commandBuffers.data());
        commandBuffers.clear();
    }

    void CrpRenderer::recreateSwapChain() {
        auto extent = crpWindow.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = crpWindow.getExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(crpDevice.device());

        if (crpSwapChain == nullptr) {
            crpSwapChain = std::make_unique<CrpSwapChain>(crpDevice, extent);
        } else {
            std::shared_ptr<CrpSwapChain> oldSwapChain = std::move(crpSwapChain);
            crpSwapChain = std::make_unique<CrpSwapChain>(crpDevice, extent, std::move(crpSwapChain));

            if (!oldSwapChain->compareSwapFormats(*crpSwapChain.get())) {
                throw std::runtime_error("Swap chain image(or depth) format has changed");
            }
        }
    }

    void CrpRenderer::createCommandBuffers() {
        commandBuffers.resize(CrpSwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = crpDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(crpDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    VkCommandBuffer CrpRenderer::beginFrame() {
        assert(!isFrameStarted && "Can't call beginFrame While already in progress");
        auto result = crpSwapChain->acquireNextImage(&currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        isFrameStarted = true;
        auto commandBuffer = getCurrentCommandBuffer();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }
        return commandBuffer;
    }

    void CrpRenderer::endFrame() {
        assert(isFrameStarted && "Can't call endFrame While frame is not in progress");
        auto commandBuffer = getCurrentCommandBuffer();
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }

        auto result = crpSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
            crpWindow.wasWindowResized()) {
            crpWindow.resetWindowResizedFlag();
            recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % CrpSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void CrpRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Can't call beginSwapChainRenderPass While frame is not in progress");
        assert(
                commandBuffer == getCurrentCommandBuffer() &&
                "Can't begin render pass on command buffer from a different frame");
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = crpSwapChain->getRenderPass();
        renderPassInfo.framebuffer = crpSwapChain->getFrameBuffer(currentImageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = crpSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t >(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(crpSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(crpSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, crpSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void CrpRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Can't call endSwapChainRenderPass While frame is not in progress");
        assert(
                commandBuffer == getCurrentCommandBuffer() &&
                "Can't begin render pass on command buffer from a different frame");
        vkCmdEndRenderPass(commandBuffer);
    }
}
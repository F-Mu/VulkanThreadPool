#include "render_system.hpp"
#include "core/macro.hpp"

//std
#include <stdexcept>
#include <array>
#include <cassert>
#include <iostream>
#include "function/global/global_context.hpp"
#include "function/framework/component/render_component.hpp"
#include "resources/systems/thread_pool_system.hpp"

namespace crp {
    RenderSystem::RenderSystem(WindowSystem &window, RenderDevice &device) : windowSystem{window}, renderDevice{device} {
        recreateSwapChain();
        createCommandBuffers();
    }

    RenderSystem::~RenderSystem() {
        freeCommandBuffers();
    }

    void RenderSystem::freeCommandBuffers() {
        vkFreeCommandBuffers(
                renderDevice.device(),
                renderDevice.getCommandPool(),
                static_cast<uint32_t>(commandBuffers.size()),
                commandBuffers.data());
        commandBuffers.clear();
    }

    void RenderSystem::recreateSwapChain() {
        auto extent = windowSystem.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = windowSystem.getExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(renderDevice.device());

        if (renderSwapChain == nullptr) {
            renderSwapChain = std::make_unique<RenderSwapChain>(renderDevice, extent);
        } else {
            std::shared_ptr<RenderSwapChain> oldSwapChain = std::move(renderSwapChain);
            renderSwapChain = std::make_unique<RenderSwapChain>(renderDevice, extent, std::move(renderSwapChain));

            if (!oldSwapChain->compareSwapFormats(*renderSwapChain)) {
                throw std::runtime_error("Swap chain image(or depth) format has changed");
            }
        }
    }

    void RenderSystem::createCommandBuffers() {
        commandBuffers.resize(RenderSwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = renderDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(renderDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    VkCommandBuffer RenderSystem::beginFrame() {
        assert(!isFrameStarted && "Can't call beginFrame While already in progress");
        auto result = renderSwapChain->acquireNextImage(&currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        isFrameStarted = true;
        nowCommandBuffer = std::make_shared<VkCommandBuffer>(getCurrentCommandBuffer());

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if (vkBeginCommandBuffer(*nowCommandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }
        return *nowCommandBuffer;
    }

    void RenderSystem::endFrame() {
        assert(isFrameStarted && "Can't call endFrame While frame is not in progress");

        if (vkEndCommandBuffer(*nowCommandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }

        auto result = renderSwapChain->submitCommandBuffers(&(*nowCommandBuffer), &currentImageIndex);
//        nowCommandBuffer.reset();
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
            windowSystem.wasWindowResized()) {
            windowSystem.resetWindowResizedFlag();
            recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % RenderSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void RenderSystem::beginSwapChainRenderPass() {
        assert(isFrameStarted && "Can't call beginSwapChainRenderPass While frame is not in progress");
        assert(
                *nowCommandBuffer == getCurrentCommandBuffer() &&
                "Can't begin render pass on command buffer from a different frame");
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderSwapChain->getRenderPass();
        renderPassInfo.framebuffer = renderSwapChain->getFrameBuffer(currentImageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = renderSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t >(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(*nowCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(renderSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(renderSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, renderSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(*nowCommandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(*nowCommandBuffer, 0, 1, &scissor);
    }

    void RenderSystem::endSwapChainRenderPass() {
        assert(isFrameStarted && "Can't call endSwapChainRenderPass While frame is not in progress");
        assert(
                *nowCommandBuffer == getCurrentCommandBuffer() &&
                "Can't begin render pass on command buffer from a different frame");
        vkCmdEndRenderPass(*nowCommandBuffer);
    }
}
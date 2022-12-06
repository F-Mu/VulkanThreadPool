#include "first_app.hpp"
#include "function/controller/keyboard_controller.hpp"
#include "resources/systems/simple_render_pass.hpp"
#include "render_buffer.hpp"
#include "function/global/global_context.hpp"
#include "function/framework/component/mesh_component.hpp"
#include "function/framework/component/render_component.hpp"
//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
//std
#include <iostream>
#include <chrono>

namespace crp {
    FirstApp::FirstApp() {
        globalContext.startEngine();
//        test();
    }

    FirstApp::~FirstApp() {}

    void FirstApp::run() {
        auto &runTimeSystem = globalContext.runTimeSystem;
        auto &taskQueueSystem = globalContext.runTimeSystem->taskQueueSystem;
        auto &threadPoolSystem = globalContext.runTimeSystem->threadPoolSystem;
        auto &gameObjectManager = globalContext.gameObjectManager;
        auto &windowSystem = globalContext.windowSystem;
        auto &device = *globalContext.device;
        auto &renderSystem = globalContext.renderSystem;
        auto &simpleRenderPass = globalContext.simpleRenderPass;
        auto &globalResources = globalContext.globalResources;
        KeyboardController controller{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!windowSystem->shouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            frameTime = glm::min(frameTime, FRAME_TIME);

            controller.addTask(windowSystem->getGLFWwindow());
            if (renderSystem->beginFrame()) {
                int frameIndex = renderSystem->getFrameIndex();
                RenderFrameInfo frameInfo{
                        frameIndex,
                        frameTime,
                        *renderSystem->nowCommandBuffer,
                        globalResources->globalDescriptorSets[frameIndex],
                        globalContext.gameObjectManager->gameObjects,
                };
                //render
                renderSystem->beginSwapChainRenderPass();
                //order here matters
                runTimeSystem->tick();
                simpleRenderPass->tick(frameInfo);
                renderSystem->endSwapChainRenderPass();
                renderSystem->endFrame();
            }
        }
        globalContext.shutdownEngine();
    }

    void FirstApp::test() {
//        auto &runTimeSystem = globalContext.runTimeSystem;
//        auto &taskQueueSystem = globalContext.runTimeSystem->taskQueueSystem;
//        auto &threadPoolSystem = globalContext.runTimeSystem->threadPoolSystem;
//        auto &gameObjectManager = globalContext.gameObjectManager;
//        auto &windowSystem = globalContext.windowSystem;
//        auto &device = *globalContext.device;
//        auto &renderSystem = globalContext.renderSystem;
//        auto &simpleRenderPass = globalContext.simpleRenderPass;
//        auto &globalResources = globalContext.globalResources;
    }

}
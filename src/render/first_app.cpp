#include "first_app.hpp"
#include "function/controller/keyboard_controller.hpp"
#include "resources/systems/simple_render_system.hpp"
#include "crp_camera.hpp"
#include "crp_buffer.hpp"
#include "function/global/global_context.hpp"
#include "resources/systems/thread_pool_system.hpp"
#include "resources/systems/runtime_system.hpp"
#include "resources/systems/task_queue_system.hpp"
#include "resources/manager/move_task_manager.hpp"
//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
//std
#include <stdexcept>
#include <array>
#include <cassert>
#include <iostream>
#include <chrono>
#include <thread>

namespace crp {

    FirstApp::FirstApp() {
        globalPool = CrpDescriptorPool::Builder(crpDevice)
                .setMaxSets(CrpSwapChain::MAX_FRAMES_IN_FLIGHT)
                .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, CrpSwapChain::MAX_FRAMES_IN_FLIGHT)
                .build();
        globalContext.startEngine(crpDevice);
//        test();
    }

    FirstApp::~FirstApp() {}

    void FirstApp::run() {
        auto &runTimeSystem = globalContext.runTimeSystem;
        auto &taskQueueSystem = globalContext.runTimeSystem->taskQueueSystem;
        auto &threadPoolSystem = globalContext.runTimeSystem->threadPoolSystem;
        auto &moveTaskManager = globalContext.moveTaskManager;
        auto &gameObjectManager = globalContext.gameObjectManager;

        std::vector<std::unique_ptr<CrpBuffer>> uboBuffers(CrpSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); ++i) {
            uboBuffers[i] = std::make_unique<CrpBuffer>(
                    crpDevice,
                    sizeof(GlobalUbo),
                    1,
                    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

        auto globalSetLayout = CrpDescriptorSetLayout::Builder(crpDevice)
                .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
                .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(CrpSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); ++i) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            CrpDescriptorWriter(*globalSetLayout, *globalPool)
                    .writeBuffer(0, &bufferInfo)
                    .build(globalDescriptorSets[i]);
        }

        SimpleRenderSystem simpleRenderSystem{crpDevice, crpRenderer.getSwapChainRenderPass(),
                                              globalSetLayout->getDescriptorSetLayout()};
        CrpCamera camera{};
        camera.setViewTarget(glm::vec3(0.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

        auto viewerObject = CrpGameObject::createGameObject();
        viewerObject.transform.translation.z = -2.5f;
        KeyboardController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!crpWindow.shouldClose()) {
            glfwPollEvents();
            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            frameTime = glm::min(frameTime, FRAME_TIME);

            cameraController.moveInPlaneXZ(crpWindow.getGLFWwindow(), frameTime, viewerObject);
            cameraController.addTask(crpWindow.getGLFWwindow());
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            camera.setOrthographicProjection(-2, 2, -2, 2, -3, 3);
            if (auto commandBuffer = crpRenderer.beginFrame()) {
                int frameIndex = crpRenderer.getFrameIndex();
                FrameInfo frameInfo{
                        frameIndex,
                        frameTime,
                        commandBuffer,
                        camera,
                        globalDescriptorSets[frameIndex],
                        globalContext.gameObjectManager->gameObjects,
                };

                /** 测试异步执行 如果任务完成则输出，否则不输出**/
//                static int x = 0;
//                if (x == 0 || x >= 30) {
//                    int time = 1;
//                    static auto now = globalContext.runTimeSystem->taskQueueSystem->add(
//                            [time] {
//                                std::this_thread::sleep_for(std::chrono::seconds(time));
//                                return 1;
//                            });
//                    globalContext.runTimeSystem->taskQueueSystem->add(
//                            [time] {
//                                std::this_thread::sleep_for(std::chrono::seconds(time));
//                                return 1;
//                            });
//                    if (x == 30 || x == 300) {
//                        auto result = now->getResult(true);
//                        if (result.has_value())
//                            std::cout << *result << '#' << std::endl;
//                    }
//                }
//                ++x;

                //update
                GlobalUbo ubo{};
                ubo.projection = camera.getProjection();
                ubo.view = camera.getView();
                ubo.inverseView = camera.getInverseView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();
                //order

                //render
                crpRenderer.beginSwapChainRenderPass(commandBuffer);

                //order here matters
                runTimeSystem->tick(frameInfo);
                moveTaskManager->tick(frameInfo);
                simpleRenderSystem.tick(frameInfo);
                crpRenderer.endSwapChainRenderPass(commandBuffer);
                crpRenderer.endFrame();
            }
//            gameObjectManager->tick();
        }
//        gameObjectManager->tick();

        vkDeviceWaitIdle(crpDevice.device());
        globalContext.shutdownEngine();
    }

    void FirstApp::test() {
//        auto &runTimeSystem = globalContext.runTimeSystem;
//        auto &taskQueueSystem = globalContext.runTimeSystem->taskQueueSystem;
//        auto &threadPoolSystem = globalContext.runTimeSystem->threadPoolSystem;
//        auto &moveTaskManager = globalContext.moveTaskManager;
//        auto &gameObjectManager = globalContext.gameObjectManager;
//        taskQueueSystem->add()
//        int x = 1;
//        taskQueueSystem->add(fun(),x);
//        runTimeSystem->add([])
//        taskQueueSystem->add([](int answer) { return answer; }, 1);
//        globalContext.moveTaskManager->addMoveTask(threadPoolSystem->threads[0], to);
//        globalContext.taskQueueSystem->addTask(crpDevice, globalContext.gameObjectManager);
//        globalContext.taskQueueSystem->addRunTask(globalContext.taskQueueSystem->tasks[0],
//                                                  globalContext.runTimeSystem->points[0]);
//        globalContext.taskQueueSystem->addDeleteTask(globalContext.taskQueueSystem->tasks[1]);
//        std::vector<CrpModel::Vertex> vertices{
//                {{-0.5f, -0.5f, 0}, {1., 0., 0.}},
//                {{-0.3f, -0.5f, 0}, {1., 0., 0.}},
//                {{-0.3f, -0.3f, 0}, {0., 1., 0.}},
////                {{-0.1f,0.1,0},{0.,0.,1.}},
////                {{-0.1f,-0.1f,0},{1.,0.,0.}},
////                {{0.1f,0.1f,0},{0.,1.,0.}},
//        };
//        float layer = 1;
//        float deplayer = -3;
//
//        auto rect = CrpGameObject::makeRectangle(crpDevice,
//                                                 {-0.5f, -0.5f, QUEUE_LAYER}, {-0.3f, -0.5f, QUEUE_LAYER}, {-0.5f, -0.3f, QUEUE_LAYER},
//                                                 {-0.3f, -0.3f, QUEUE_LAYER}, true, {1, 0, 0});
//
//        gameObjectManager->gameObjects.emplace(rect.getId(), std::move(rect));
//        rect = CrpGameObject::makeRectangle(crpDevice,
//                                            {-0.4f, -0.5f, TASK_LAYER}, {-0.3f, -0.5f, TASK_LAYER},
//                                            {-0.5f, -0.3f, TASK_LAYER},
//                                            {-0.3f, -0.3f, TASK_LAYER}, true, {0, 0.5, 0});
//
//        gameObjectManager->gameObjects.emplace(rect.getId(), std::move(rect));
//
//        rect = CrpGameObject::makeRectangle(crpDevice,
//                                            {-0.4f, -0.5f, THREAD_LAYER}, {-0.3f, -0.5f, THREAD_LAYER},
//                                            {-0.5f, -0.3f, THREAD_LAYER},
//                                            {-0.3f, -0.3f, THREAD_LAYER}, true, {0, 0, 1});
//
//        gameObjectManager->gameObjects.emplace(rect.getId(), std::move(rect));
//        rect = CrpGameObject::makeRectangle(crpDevice,
//                                            {-0.6f, -0.6f, layer}, {-0.2f, -0.6f, layer}, {-0.6f, -0.2f, layer},
//                                            {-0.2f, -0.2f, layer}, false);
//
//        gameObjectManager->gameObjects.emplace(rect.getId(), std::move(rect));

    }

}
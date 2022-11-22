#include "first_app.hpp"
#include "keyboard_movement_controller.hpp"
#include "systems/simple_render_system.hpp"
#include "crp_camera.hpp"
#include "crp_buffer.hpp"
#include "global/global_context.hpp"
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

float MAX_FRAME_TIME = 60;
namespace crp {
    void input() {
        int x;
        while (true) {
            std::cout << "You can add task(0)" << std::endl;
            std::cout << "You can quit(1)" << std::endl;
            std::cin >> x;
        }
    }

    FirstApp::FirstApp() {
        globalPool = CrpDescriptorPool::Builder(crpDevice)
                .setMaxSets(CrpSwapChain::MAX_FRAMES_IN_FLIGHT)
                .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, CrpSwapChain::MAX_FRAMES_IN_FLIGHT)
                .build();
//        loadGameObjects();
        globalContext.startEngine(crpDevice);
        test();
    }

    FirstApp::~FirstApp() {}

    void FirstApp::run() {
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
//        camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
        camera.setViewTarget(glm::vec3(0.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

        auto viewerObject = CrpGameObject::createGameObject();
        viewerObject.transform.translation.z = -2.5f;
        KeyboardMovementController cameraController{};

//        std::thread th(input);
        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!crpWindow.shouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            frameTime = glm::min(frameTime, MAX_FRAME_TIME);

            cameraController.moveInPlaneXZ(crpWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

//            float aspect = crpRenderer.getAspectRatio();
            camera.setOrthographicProjection(-2, 2, -2, 2, -5, 5);
//            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);
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

                //update
                GlobalUbo ubo{};
                ubo.projection = camera.getProjection();
                ubo.view = camera.getView();
                ubo.inverseView = camera.getInverseView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();
                globalContext.threadPoolSystem->tick(frameInfo);
                globalContext.taskQueueSystem->tick(frameInfo);

                //render
                crpRenderer.beginSwapChainRenderPass(commandBuffer);

                //order here matters
                simpleRenderSystem.renderGameObjects(frameInfo);
                crpRenderer.endSwapChainRenderPass(commandBuffer);
                crpRenderer.endFrame();
            }
        }
        vkDeviceWaitIdle(crpDevice.device());
        globalContext.shutdownEngine();
    }


    void FirstApp::test() {
        glm::vec3 to = globalContext.runTimeSystem->points[0];
        globalContext.threadPoolSystem->addMoveTask(globalContext.threadPoolSystem->threads[0].rectangle, to);
        globalContext.taskQueueSystem->addTask(crpDevice, globalContext.gameObjectManager);
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
//                                                 {-0.5f, -0.5f, layer}, {-0.3f, -0.5f, layer}, {-0.5f, -0.3f, layer},
//                                                 {-0.3f, -0.3f, layer}, true, {0.5, 0, 0});
//
//        gameObjectManager->gameObjects.emplace(rect.getId(), std::move(rect));
//        rect = CrpGameObject::makeRectangle(crpDevice,
//                                            {-0.4f, -0.5f, deplayer}, {-0.3f, -0.5f, deplayer},
//                                            {-0.5f, -0.3f, deplayer},
//                                            {-0.3f, -0.3f, deplayer}, true, {0, 0.5, 0});
//
//        gameObjectManager->gameObjects.emplace(rect.getId(), std::move(rect));
//        rect = CrpGameObject::makeRectangle(crpDevice,
//                                            {-0.6f, -0.6f, layer}, {-0.2f, -0.6f, layer}, {-0.6f, -0.2f, layer},
//                                            {-0.2f, -0.2f, layer}, false);
//
//        gameObjectManager->gameObjects.emplace(rect.getId(), std::move(rect));

    }

}
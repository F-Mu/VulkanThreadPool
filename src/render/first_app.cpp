#include "first_app.hpp"
#include "function/controller/keyboard_controller.hpp"
#include "resources/systems/simple_render_pass.hpp"
#include "crp_camera.hpp"
#include "crp_buffer.hpp"
#include "function/global/global_context.hpp"
#include "resources/systems/thread_pool_system.hpp"
#include "resources/systems/runtime_system.hpp"
#include "resources/systems/task_queue_system.hpp"
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

//        CrpCamera camera{};
//        camera.setViewTarget(glm::vec3(0.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

        KeyboardController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!windowSystem->shouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            frameTime = glm::min(frameTime, FRAME_TIME);

//            cameraController.moveInPlaneXZ(crpWindow.getGLFWwindow(), frameTime, viewerObject);
            cameraController.addTask(windowSystem->getGLFWwindow());
//            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

//            camera.setOrthographicProjection(-2, 2, -2, 2, -3, 3);
            renderSystem->beginFrame();
//            if (renderSystem->nowCommandBuffer) {
            int frameIndex = renderSystem->getFrameIndex();
            FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    renderSystem->nowCommandBuffer,
//                        camera,
                    globalResources->globalDescriptorSets[frameIndex],
                    globalContext.gameObjectManager->gameObjects,
            };

            //render
            renderSystem->beginSwapChainRenderPass(renderSystem->nowCommandBuffer);


            //order here matters
            runTimeSystem->tick(frameInfo);
            simpleRenderPass->tick(frameInfo);
//                moveTaskManager->tick(frameInfo);
            renderSystem->endSwapChainRenderPass(renderSystem->nowCommandBuffer);
            renderSystem->endFrame();
//            }
        }
//        gameObjectManager->tick();
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
//        globalContext.taskQueueSystem->addTask(device, globalContext.gameObjectManager);
//        globalContext.taskQueueSystem->addRunTask(globalContext.taskQueueSystem->tasks[0],
//                                                  globalContext.runTimeSystem->points[0]);
//        globalContext.taskQueueSystem->addDeleteTask(globalContext.taskQueueSystem->tasks[1]);
//        std::vector<CrpModel::Vertex> vertices{
//                {{-0.5f, -0.5f, 0}, {1., 0., 0.}},
//                {{-0.3f, -0.5f, 0}, {1., 0., 0.}},
//                {{-0.3f, -0.3f, 0}, {0., 1., 0.}},
//                {{-0.1f,0.1,0},{0.,0.,1.}},
//                {{-0.1f,-0.1f,0},{1.,0.,0.}},
//                {{0.1f,0.1f,0},{0.,1.,0.}},
//        };
//        float layer = 1;
//        float deplayer = -3;
//
//        auto rect = CrpGameObject::makeRectangle(device,
//                                                 {-0.5f, -0.5f, QUEUE_LAYER}, {-0.3f, -0.5f, QUEUE_LAYER}, {-0.5f, -0.3f, QUEUE_LAYER},
//                                                 {-0.3f, -0.3f, QUEUE_LAYER}, true, {1, 0, 0});
//
//        gameObjectManager->gameObjects.emplace(rect.getId(), std::move(rect));
//        rect = CrpGameObject::makeRectangle(device,
//                                            {-0.4f, -0.5f, TASK_LAYER}, {-0.3f, -0.5f, TASK_LAYER},
//                                            {-0.5f, -0.3f, TASK_LAYER},
//                                            {-0.3f, -0.3f, TASK_LAYER}, true, {0, 0.5, 0});
//
//        gameObjectManager->gameObjects.emplace(rect.getId(), std::move(rect));
//
//        rect = CrpGameObject::makeRectangle(device,
//                                            {-0.4f, -0.5f, THREAD_LAYER}, {-0.3f, -0.5f, THREAD_LAYER},
//                                            {-0.5f, -0.3f, THREAD_LAYER},
//                                            {-0.3f, -0.3f, THREAD_LAYER}, true, {0, 0, 1});
//
//        gameObjectManager->gameObjects.emplace(rect.getId(), std::move(rect));
//        rect = CrpGameObject::makeRectangle(device,
//                                            {-0.6f, -0.6f, layer}, {-0.2f, -0.6f, layer}, {-0.6f, -0.2f, layer},
//                                            {-0.2f, -0.2f, layer}, false);
//
//        gameObjectManager->gameObjects.emplace(rect.getId(), std::move(rect));

    }

}
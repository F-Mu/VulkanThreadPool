#include "first_app.hpp"
#include "keyboard_movement_controller.hpp"
#include "systems/simple_render_system.hpp"
#include "systems/point_light_system.hpp"
#include "crp_camera.hpp"
#include "crp_buffer.hpp"
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

float MAX_FRAME_TIME = 60;
namespace crp {

    FirstApp::FirstApp() {
        globalPool = CrpDescriptorPool::Builder(crpDevice)
                .setMaxSets(CrpSwapChain::MAX_FRAMES_IN_FLIGHT)
                .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, CrpSwapChain::MAX_FRAMES_IN_FLIGHT)
                .build();
        loadGameObjects();
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
        PointLightSystem pointLightSystem{crpDevice, crpRenderer.getSwapChainRenderPass(),
                                          globalSetLayout->getDescriptorSetLayout()};
        CrpCamera camera{};
//        camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
        camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

        auto viewerObject = CrpGameObject::createGameObject();
        viewerObject.transform.translation.z = -2.5f;
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!crpWindow.shouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            frameTime = glm::min(frameTime, MAX_FRAME_TIME);

            cameraController.moveInPlaneXZ(crpWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = crpRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);
            if (auto commandBuffer = crpRenderer.beginFrame()) {
                int frameIndex = crpRenderer.getFrameIndex();
                FrameInfo frameInfo{
                        frameIndex,
                        frameTime,
                        commandBuffer,
                        camera,
                        globalDescriptorSets[frameIndex],
                        gameObjects,
                };

                //update
                GlobalUbo ubo{};
                ubo.projection = camera.getProjection();
                ubo.view = camera.getView();
                ubo.inverseView = camera.getInverseView();
                pointLightSystem.update(frameInfo, ubo);
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                //render
                crpRenderer.beginSwapChainRenderPass(commandBuffer);

                //order here matters
                simpleRenderSystem.renderGameObjects(frameInfo);
                pointLightSystem.render(frameInfo);
                crpRenderer.endSwapChainRenderPass(commandBuffer);
                crpRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(crpDevice.device());
    }


    void FirstApp::loadGameObjects() {
        std::shared_ptr<CrpModel> crpModel = CrpModel::createModelFromFile(
                crpDevice, "models/flat_vase.obj");
        auto flatVase = CrpGameObject::createGameObject();
        flatVase.model = crpModel;
        flatVase.transform.translation = {-.5f, .5f, 0.f};
        flatVase.transform.scale = glm::vec3{3.f, 1.5f, 3.f};
        gameObjects.emplace(flatVase.getId(), std::move(flatVase));

        crpModel = CrpModel::createModelFromFile(
                crpDevice, "models/smooth_vase.obj");
        auto smoothVase = CrpGameObject::createGameObject();
        smoothVase.model = crpModel;
        smoothVase.transform.translation = {.5f, .5f, 0.f};
        smoothVase.transform.scale = glm::vec3{3.f, 1.5f, 3.f};
        gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

        crpModel = CrpModel::createModelFromFile(
                crpDevice, "models/quad.obj");
        auto floor = CrpGameObject::createGameObject();
        floor.model = crpModel;
        floor.transform.translation = {0.f, .5f, 0.f};
        floor.transform.scale = glm::vec3{3.f, 1.f, 3.f};
        gameObjects.emplace(floor.getId(), std::move(floor));

        std::vector<glm::vec3> lightColors{
                {1.f, .1f, .1f},
                {.1f, .1f, 1.f},
                {.1f, 1.f, .1f},
                {1.f, 1.f, .1f},
                {.1f, 1.f, 1.f},
                {1.f, 1.f, 1.f}  //
        };

        for (int i = 0; i < lightColors.size(); ++i) {
            auto pointLight = CrpGameObject::makePointLight(0.2f);
            pointLight.color = lightColors[i];
            auto rotateLight = glm::rotate(
                    glm::mat4(1.f),
                    (i * glm::two_pi<float>()) / lightColors.size(),
                    {0.f, -1.f, 0.f});
            pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
            gameObjects.emplace(pointLight.getId(), std::move(pointLight));
        }
    }
}
#include "render_component.hpp"
#include "function/global/global_context.hpp"
#include "function/framework/crp_game_obejct.hpp"
#include "resources/systems/simple_render_pass.hpp"
#include "mesh_component.hpp"
#include "core/push_constant.hpp"
namespace crp {
    RenderComponent::RenderComponent(const std::weak_ptr<CrpGameObject> &parent, CrpDevice &device) :
            Component(parent), crpDevice{device} {
        type = "RenderComponent";
        auto meshComponent = parent.lock()->tryGetComponentConst(MeshComponent);
        if (!meshComponent)return;
        if (meshComponent->worldPoints.empty())return;
        createVertexBuffers(meshComponent->worldPoints);
        if (meshComponent->worldIndices.empty())return;
        createIndexBuffers(meshComponent->worldIndices);
    }

    RenderComponent::~RenderComponent() {
        vertexBuffer.reset();
        indexBuffer.reset();
    }

//    void RenderComponent::update() {
//        auto meshComponent = m_parent_object.lock()->tryGetComponent(MeshComponent);
//        if (!meshComponent)return;
//        meshComponent->tick();
//        if (meshComponent->worldPoints.empty())return;
//        vertexBuffer.reset();
//        createVertexBuffers(meshComponent->worldPoints);
//        if (meshComponent->worldIndices.empty())return;
//        indexBuffer.reset();
//        createIndexBuffers(meshComponent->worldIndices);
//    }

    void RenderComponent::createVertexBuffers(const std::vector<Vertex> &vertices) {
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "Vertex count must be at least 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
        uint32_t vertexSize = sizeof(vertices[0]);

        CrpBuffer stagingBuffer{
                crpDevice,
                vertexSize,
                vertexCount,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        };

        stagingBuffer.map();
        stagingBuffer.writeToBuffer((void *) vertices.data());

        vertexBuffer = std::make_unique<CrpBuffer>(
                crpDevice,
                vertexSize,
                vertexCount,
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );
        crpDevice.copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);
    }

    void RenderComponent::createIndexBuffers(const std::vector<uint32_t> &indices) {
        indexCount = static_cast<uint32_t>(indices.size());
        hasIndexBuffer = indexCount > 0;

        if (!hasIndexBuffer) {
            return;
        }

        VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
        uint32_t indexSize = sizeof(indices[0]);

        CrpBuffer stagingBuffer{
                crpDevice,
                indexSize,
                indexCount,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        };

        stagingBuffer.map();
        stagingBuffer.writeToBuffer((void *) indices.data());

        indexBuffer = std::make_unique<CrpBuffer>(
                crpDevice,
                indexSize,
                indexCount,
                VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        crpDevice.copyBuffer(stagingBuffer.getBuffer(), indexBuffer->getBuffer(), bufferSize);
    }

    void RenderComponent::draw(VkCommandBuffer commandBuffer) {
        if (hasIndexBuffer) {
            vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
        } else {
            vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
        }
    }

    void RenderComponent::bind(VkCommandBuffer commandBuffer) {
        VkBuffer buffers[] = {vertexBuffer->getBuffer()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
        if (hasIndexBuffer) {
            vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
        }
    }

    void RenderComponent::tick() {
        auto transform = m_parent_object.lock()->tryGetComponent(TransformComponent);
        SimplePushConstantData push{};
        push.modelMatrix = transform->mat4();
        push.normalMatrix = transform->normalMatrix();

        vkCmdPushConstants(
                globalContext.renderSystem->nowCommandBuffer,
                globalContext.simpleRenderPass->pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push
        );
        bind(globalContext.renderSystem->nowCommandBuffer);
        draw(globalContext.renderSystem->nowCommandBuffer);
    }

}
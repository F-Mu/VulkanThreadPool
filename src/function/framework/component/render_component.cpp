#include "render_component.hpp"
#include "function/global/global_context.hpp"
#include "function/framework/game_object.hpp"
#include "resources/systems/simple_render_pass.hpp"
#include "mesh_component.hpp"
#include "core/push_constant.hpp"

namespace crp {
    Model::Model(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices) {
        createVertexBuffers(vertices);
        createIndexBuffers(indices);
    }

    Model::~Model() {
        vertexBuffer.reset();
        indexBuffer.reset();
    }

    void Model::createVertexBuffers(const std::vector<Vertex> &vertices) {
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "Vertex count must be at least 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
        uint32_t vertexSize = sizeof(vertices[0]);

        RenderBuffer stagingBuffer{
                *globalContext.device,
                vertexSize,
                vertexCount,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        };

        stagingBuffer.map();
        stagingBuffer.writeToBuffer((void *) vertices.data());

        vertexBuffer = std::make_unique<RenderBuffer>(
                *globalContext.device,
                vertexSize,
                vertexCount,
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );
        globalContext.device->copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);
    }

    void Model::createIndexBuffers(const std::vector<uint32_t> &indices) {
        indexCount = static_cast<uint32_t>(indices.size());
        hasIndexBuffer = indexCount > 0;

        if (!hasIndexBuffer) {
            return;
        }

        VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
        uint32_t indexSize = sizeof(indices[0]);

        RenderBuffer stagingBuffer{
                *globalContext.device,
                indexSize,
                indexCount,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        };

        stagingBuffer.map();
        stagingBuffer.writeToBuffer((void *) indices.data());

        indexBuffer = std::make_unique<RenderBuffer>(
                *globalContext.device,
                indexSize,
                indexCount,
                VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );
        globalContext.device->copyBuffer(stagingBuffer.getBuffer(), indexBuffer->getBuffer(), bufferSize);
    }

    void Model::draw(VkCommandBuffer commandBuffer) {
        if (hasIndexBuffer) {
            vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
        } else {
            vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
        }
    }

    void Model::bind(VkCommandBuffer commandBuffer) {
        VkBuffer buffers[] = {vertexBuffer->getBuffer()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
        if (hasIndexBuffer) {
            vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
        }
    }

    RenderComponent::RenderComponent(const std::weak_ptr<GameObject> &parent) :
            Component(parent) {
        type = "RenderComponent";
        auto meshComponent = parent.lock()->tryGetComponentConst(MeshComponent);
        if (!meshComponent)return;
        if (meshComponent->vertexPoints.empty())return;
        if (meshComponent->worldIndices.empty())return;
        model = std::make_shared<Model>(meshComponent->vertexPoints, meshComponent->worldIndices);
    }

    RenderComponent::~RenderComponent() {
        model.reset();
    }

    void RenderComponent::update() {
        auto meshComponent = m_parent_object.lock()->tryGetComponentConst(MeshComponent);
        if (!meshComponent || !meshComponent->isDirty())return;
        if (meshComponent->vertexPoints.empty())return;
        if (meshComponent->worldIndices.empty())return;
        model = std::make_shared<Model>(meshComponent->vertexPoints, meshComponent->worldIndices);
    }

    void RenderComponent::tick() {
        if (!model || isDirty())return;
        update();
        auto transform = m_parent_object.lock()->tryGetComponent(TransformComponent);
        SimplePushConstantData push{};
        push.modelMatrix = transform->mat4();
        push.normalMatrix = transform->normalMatrix();

        vkCmdPushConstants(
                *globalContext.renderSystem->nowCommandBuffer,
                globalContext.simpleRenderPass->pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push
        );
        model->bind(*globalContext.renderSystem->nowCommandBuffer);
        model->draw(*globalContext.renderSystem->nowCommandBuffer);
    }
}
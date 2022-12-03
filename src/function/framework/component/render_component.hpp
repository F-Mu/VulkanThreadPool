#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "render/crp_device.hpp"
#include "render/crp_buffer.hpp"
#include "component.hpp"
#include "core/vertex.hpp"

namespace crp {
    class RenderComponent : public Component {
    public:
        RenderComponent(const std::weak_ptr<CrpGameObject> &parent,CrpDevice &device);
        ~RenderComponent();

        void bind(VkCommandBuffer commandBuffer);

        void draw(VkCommandBuffer commandBuffer);

        void tick();

    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);

        void createIndexBuffers(const std::vector<uint32_t> &indices);

        CrpDevice &crpDevice;

        std::unique_ptr<CrpBuffer> vertexBuffer;
        uint32_t vertexCount;

        bool hasIndexBuffer = false;
        std::unique_ptr<CrpBuffer> indexBuffer;
        uint32_t indexCount;
    };
}
#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "render/crp_device.hpp"
#include "render/crp_buffer.hpp"
#include "component.hpp"
#include "core/vertex.hpp"

namespace crp {

//    struct Vertex {
//        glm::vec3 position{};
//        glm::vec3 color{};
//        glm::vec3 normal{};
//        glm::vec2 uv{};
//
//        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
//
//        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
//
//        bool operator==(const Vertex &other) const {
//            return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
//        }
//    };

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
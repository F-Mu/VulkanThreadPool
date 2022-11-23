#pragma once

#include "crp_device.hpp"
#include "crp_buffer.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
//std
#include <vector>
#include <memory>

namespace crp {
    class CrpModel {
    public:

        struct Vertex {
            glm::vec3 position{};
            glm::vec3 color{};
            glm::vec3 normal{};
            glm::vec2 uv{};

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();

            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            bool operator==(const Vertex &other) const {
                return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
            }
        };
        CrpModel(CrpDevice &device, const std::vector<Vertex> &vertices);

        ~CrpModel();

        CrpModel(const CrpModel &) = delete;

        CrpModel &operator=(const CrpModel &) = delete;

        static std::unique_ptr<CrpModel> createModelFromVertices(
                CrpDevice &device, const std::vector<Vertex> &vertices);

        void bind(VkCommandBuffer commandBuffer);

        void draw(VkCommandBuffer commandBuffer);

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
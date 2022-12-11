#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "render/render_device.hpp"
#include "render/render_buffer.hpp"
#include "component.hpp"
#include "core/vertex.hpp"

namespace crp {
    class Model {
    public:
        Model(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);

        ~Model();

        Model(const Model &) = delete;

        Model &operator=(const Model &) = delete;

        void bind(VkCommandBuffer commandBuffer);

        void draw(VkCommandBuffer commandBuffer) const;

    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);

        void createIndexBuffers(const std::vector<uint32_t> &indices);

        std::unique_ptr<RenderBuffer> vertexBuffer;
        uint32_t vertexCount;

        bool hasIndexBuffer = false;
        std::unique_ptr<RenderBuffer> indexBuffer;
        uint32_t indexCount;
    };

    class RenderComponent : public Component {
    public:
        explicit RenderComponent(const std::weak_ptr<GameObject> &parent,const std::string &type);

        ~RenderComponent();

        void update();

        void tick() override;
    private:
        std::shared_ptr<Model> model{};
    };
}
#pragma once

#include "render/crp_device.hpp"
#include "component.hpp"
#include "core/vertex.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace crp {
    class MeshComponent : public Component {
    public:
        MeshComponent(const std::weak_ptr<CrpGameObject> &parent, std::vector<glm::vec3> &_points, glm::vec3 &_color,
                      bool _fill);

        void getWorld();
        void getIndices();
//        std::unique_ptr<CrpModel> model{};
        std::vector<glm::vec3> points{};
        std::vector<Vertex> worldPoints{};
        std::vector<uint32_t> worldIndices{};
        glm::vec3 color;
        bool fill;

        void tick();
    };
}
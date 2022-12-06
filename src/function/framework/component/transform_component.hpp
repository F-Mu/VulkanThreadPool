#pragma once

#include <glm/glm.hpp>
#include "component.hpp"
#include "string"

namespace crp {
    class TransformComponent : public Component {
    public:
        TransformComponent(const std::weak_ptr<GameObject> &parent) :
                Component(parent) { type = "TransformComponent"; }

        glm::vec3 translation{};
        glm::vec3 scale{1.f, 1.f, 1.f};
        glm::vec3 rotation{};

        // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        glm::mat4 mat4();

        glm::mat3 normalMatrix();

        void setPosition(glm::vec3 point);

        void tick();
    };
}
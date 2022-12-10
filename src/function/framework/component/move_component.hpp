#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "core/macro.hpp"
#include "component.hpp"

namespace crp {
    class MoveComponent : public Component {
    public:
        glm::vec3 direction{};
        std::vector<glm::vec3> destinations;
        std::vector<float> times;
        bool move{};
        glm::vec3 &center;

        void setDestination(glm::vec3 &_destination, float _time = FRAME_TIME);

        void setDestination(std::vector<glm::vec3> &_destinations, float _time = FRAME_TIME);

        explicit MoveComponent(const std::weak_ptr<GameObject> &parent);

        void MoveToPoint(glm::vec3 &destination);

        bool isFinished() {
            if (destinations.empty()) {
                now = 0;
                move = false;
                return true;
            }
            if (now == destinations.size()) {
                now = 0;
                move = false;
                destinations.clear();
                return true;
            }
            return false;
        };

        void tick() override;

//    private:
        int now = 0;
        float speed{};
    };
}
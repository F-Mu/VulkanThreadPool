#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "core/macro.hpp"
#include "component.hpp"

namespace crp {
    class MoveComponent : public Component {
    public:
        void setDestination(glm::vec3 &_destination, float _time = FRAME_TIME);

        void setDestination(std::vector<glm::vec3> &_destinations, float _time = FRAME_TIME);

        explicit MoveComponent(const std::weak_ptr<GameObject> &parent, const std::string &type);

        void MoveToPoint(glm::vec3 &destination);

        bool isFinished() {
            if (destinations.empty()) {
                now = 0;
                return true;
            }
            if (now >= destinations.size()) {
                now = 0;
                destinations.clear();
                return true;
            }
            if (EQUAL(center, destinations[now])) {
                center = destinations[now];
                now = 0;
                destinations.clear();
            }
            return false;
        };

        void tick() override;

    private:
        glm::vec3 direction{};
        std::vector<glm::vec3> destinations{};
        glm::vec3 &center;
        int now = 0;
        float speed{};
    };
}
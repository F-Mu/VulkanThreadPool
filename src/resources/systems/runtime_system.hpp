#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "function/framework/crp_game_obejct.hpp"
#include "core/macro.hpp"
#include "render/crp_frame_info.hpp"
#include "function/framework/rectangle.hpp"

namespace crp {
    class TaskQueueSystem;

    class ThreadPoolSystem;

    class RuntimeSystem : public Rectangle {
    public:
        RuntimeSystem();

        void tick();

        void clear();

        std::vector<glm::vec3> points;
        std::shared_ptr<TaskQueueSystem> taskQueueSystem;
        std::shared_ptr<ThreadPoolSystem> threadPoolSystem;
    private:
        static constexpr float up = -0.55f, down = 0.55f, left = -0.15f, right = 0.15f;
        static constexpr glm::vec3 x{left, up, QUEUE_LAYER}, y{right, up, QUEUE_LAYER}, z{left, down, QUEUE_LAYER}
        , w{right, down, QUEUE_LAYER};
    };
}
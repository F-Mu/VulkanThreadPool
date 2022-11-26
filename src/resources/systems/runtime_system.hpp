#pragma once

#include <vector>

#include <glm/glm.hpp>
#include "render/crp_game_obejct.hpp"
#include "core/macro.hpp"
#include "task_queue_system.hpp"
#include "thread_pool_system.hpp"

namespace crp {
    class RuntimeSystem {
    public:
        RuntimeSystem(CrpDevice &crpDevice);

        void tick(FrameInfo &frameInfo);

        void clear();

        std::vector<glm::vec3> points;
        std::shared_ptr<TaskQueueSystem> taskQueueSystem;
        std::shared_ptr<ThreadPoolSystem> threadPoolSystem;

        std::queue<Task> tasksRun;
    private:
        float up = -1.1f, down = 1.1f, left = -0.3f, right = 0.3f;
        glm::vec3 x{left, up, QUEUE_LAYER}, y{right, up, QUEUE_LAYER}, z{left, down, QUEUE_LAYER}, w{right, down,
                                                                                                     QUEUE_LAYER};
        GameObjectManager::id_t id;
        CrpDevice &crpDevice;
    };
}
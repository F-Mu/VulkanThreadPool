#pragma once

#include <vector>

#include <glm/glm.hpp>
#include "../crp_game_obejct.hpp"
#include "../core/macro.hpp"
#include "../resources/game_object_manager.hpp"

namespace crp {
    class Thread {
    public:
        std::vector<glm::vec3> points;
    };

    class ThreadPoolSystem {
    public:
        ThreadPoolSystem(CrpDevice &crpDevice, const std::shared_ptr<GameObjectManager> &manager);

        std::vector<glm::vec3> points;
        std::vector<Thread> threads;
    private:
        int threadNum = 5;
        float up = -1.1f, down = 1.1f, left = -1.5f, right = -1.0f;
        glm::vec3 x{left, up, TASK_LAYER}, y{right, up, TASK_LAYER}, z{left, down, TASK_LAYER}, w{right, down,
                                                                                                  TASK_LAYER};
    };
}
#pragma once

#include <vector>

#include <glm/glm.hpp>
#include "../crp_game_obejct.hpp"
#include "../core/macro.hpp"
#include "../resources/game_object_manager.hpp"

namespace crp{
    class RuntimeSystem{
    public:
        RuntimeSystem(CrpDevice &crpDevice, const std::shared_ptr<GameObjectManager> &manager);

        std::vector<glm::vec3> points;

    private:
        float up = -1.1f, down = 1.1f, left = -0.3f, right = 0.3f;
        glm::vec3 x{left, up, QUEUE_LAYER}, y{right, up, QUEUE_LAYER}, z{left, down, QUEUE_LAYER}, w{right, down,
                                                                                                     QUEUE_LAYER};
        GameObjectManager::id_t id;
    };
}
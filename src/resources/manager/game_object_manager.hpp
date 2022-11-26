#pragma once
#include <list>
#include "render/crp_game_obejct.hpp"

namespace crp {

    class GameObjectManager {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, CrpGameObject>;
        Map gameObjects;

        void deleteById(id_t id);
        void tick();
    private:
        std::list<id_t>deleteTasks;
    };
}

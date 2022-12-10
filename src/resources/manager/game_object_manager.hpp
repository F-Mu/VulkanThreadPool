#pragma once

#include <list>
#include <map>
#include "function/framework/game_object.hpp"

namespace crp {
    class GameObjectManager {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, std::shared_ptr<GameObject>>;
        Map gameObjects;

        void deleteById(id_t id);

        void endTick();

        void tick();

        void registerGO(const std::shared_ptr<GameObject> &gameObject);

    private:
        std::list<id_t> deleteTasks;
    };
}

#pragma once

#include <list>
#include <map>
#include "function/framework/crp_game_obejct.hpp"

namespace crp {

    class GameObjectManager {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, std::shared_ptr<CrpGameObject>>;
        Map gameObjects;

        void deleteById(id_t id);

        void tick();

        void registerGO(const std::shared_ptr<CrpGameObject>&gameObject);

        void addGameObject(const id_t &id, const CrpGameObject &gameObject);

    private:
        std::list<id_t> deleteTasks;
    };
}

#include "game_object_manager.hpp"

namespace crp {
    void GameObjectManager::deleteById(id_t id) {
        gameObjects.erase(id);
//        std::unordered_map<id_t, CrpGameObject>mp;
//        mp.find(0);
//        gameObjects[id].rectangle.get();
    }
}
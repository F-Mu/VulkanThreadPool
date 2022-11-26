#include "game_object_manager.hpp"
#include <iostream>
namespace crp {
    void GameObjectManager::deleteById(id_t id) {
        deleteTasks.push_back(id);
//        std::unordered_map<id_t, CrpGameObject>mp;
//        mp.find(0);
//        gameObjects[id].rectangle.get();
    }

    void GameObjectManager::tick() {
        for (auto &id: deleteTasks) {
            gameObjects.find(id)->second.clear();
            gameObjects.erase(id);
        }
        deleteTasks.clear();
    }
}
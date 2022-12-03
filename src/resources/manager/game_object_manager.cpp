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
        for (auto &i: gameObjects)i.second->tick();
//        for (auto &id: deleteTasks) {
////            gameObjects.find(id)->second.clear();
//            gameObjects.erase(id);
//        }
//        deleteTasks.clear();
    }

    void GameObjectManager::addGameObject(const id_t &id, const CrpGameObject &gameObject) {
//        gameObjects.emplace(id, std::make_shared<CrpGameObject>(gameObject));
    }

    void GameObjectManager::registerGO(const std::shared_ptr<CrpGameObject>& gameObject) {
        gameObjects.emplace(gameObject->getId(), gameObject);
    }
}
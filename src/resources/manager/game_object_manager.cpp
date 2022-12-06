#include "game_object_manager.hpp"
#include <iostream>

namespace crp {
    void GameObjectManager::deleteById(id_t id) {
        deleteTasks.push_back(id);
    }

    void GameObjectManager::tick() {
        for (auto &i: gameObjects)i.second->tick();
    }

    void GameObjectManager::addGameObject(const id_t &id, const GameObject &gameObject) {
    }

    void GameObjectManager::registerGO(const std::shared_ptr<GameObject> &gameObject) {
        gameObjects.emplace(gameObject->getId(), gameObject);
    }
}
#include "game_object_manager.hpp"
#include <iostream>
#include "function/framework/component/delete_component.hpp"

namespace crp {
    void GameObjectManager::deleteById(id_t id) {
        deleteTasks.push_back(id);
    }

    void GameObjectManager::tick() {
        for (auto &i: gameObjects) {
            i.second->tick();
            auto j = i.second->tryGetComponent(DeleteComponent);
            if (j && j->isFinished())deleteById(i.first);
        }
    }

    void GameObjectManager::endTick() {
        for (auto &i: deleteTasks) {
            gameObjects.erase(i);
        }
    }

    void GameObjectManager::registerGO(const std::shared_ptr<GameObject> &gameObject) {
        gameObjects.emplace(gameObject->getId(), gameObject);
    }
}
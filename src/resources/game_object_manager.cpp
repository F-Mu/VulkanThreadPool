#include "game_object_manager.hpp"

namespace crp {
    void GameObjectManager::deleteById(id_t id) {
        gameObjects.erase(id);
    }
}
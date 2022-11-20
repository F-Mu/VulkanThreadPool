#include "run_time_system.hpp"

crp::RunTimeSystem::RunTimeSystem(crp::CrpDevice &crpDevice, const std::shared_ptr<GameObjectManager> &manager) {

    auto ThreadPoolRect = CrpGameObject::makeRectangle(crpDevice,
                                                       x, y, z, w, false);
    manager->gameObjects.emplace(ThreadPoolRect.getId(), std::move(ThreadPoolRect));
}

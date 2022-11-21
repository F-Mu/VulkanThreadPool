#include "runtime_system.hpp"

namespace crp {
    RuntimeSystem::RuntimeSystem(crp::CrpDevice &crpDevice, const std::shared_ptr<GameObjectManager> &manager) {

        auto RuntimeRect = CrpGameObject::makeRectangle(crpDevice,
                                                        x, y, z, w, false);
        id = RuntimeRect.getId();
        manager->gameObjects.emplace(RuntimeRect.getId(), std::move(RuntimeRect));

        float mid = (left + right) / 2;
        points.resize(THREAD_NUM);
        float hCut = (down - up) / float(THREAD_NUM);
        float yFirst = (up + hCut + up) / 2;
        for (int i = 0; i < THREAD_NUM; ++i) {
            points[i] = {mid, yFirst + hCut * i, THREAD_LAYER};
        }
    }

}

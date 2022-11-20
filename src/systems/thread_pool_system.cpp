#include "thread_pool_system.hpp"
#include <iostream>

namespace crp {
    ThreadPoolSystem::ThreadPoolSystem(CrpDevice &crpDevice, const std::shared_ptr<GameObjectManager> &manager) {
        auto ThreadPoolRect = CrpGameObject::makeRectangle(crpDevice,
                                                           x, y, z, w, false);
        manager->gameObjects.emplace(ThreadPoolRect.getId(), std::move(ThreadPoolRect));
        float mid = (left + right) / 2;
        points.resize(threadNum);
        threads.resize(threadNum);
        float hCut = (down - up) / float(threadNum);
        float yFirst = (up + hCut + up) / 2;
        float threadWidth = right - left - .1f;
        float threadHeight = (hCut - .1f);
        float threadLeft = mid - threadWidth / 2;
        float threadRight = mid + threadWidth / 2;
        float threadUp = yFirst - threadHeight / 2;
        float threadDown = threadUp + threadHeight;
        for (int i = 0; i < threadNum; ++i) {
            points[i] = {mid, yFirst + hCut * i, TASK_LAYER};
            threads[i].points.resize(4);
            float l = threadLeft, r = threadRight,
                    u = threadUp + hCut * i, d = threadDown + hCut * i;
//            std::cout << l << ' ' << r << ' ' << u << ' ' << d << ' ' << threadUp << ' ' << threadHeight << ' ' << hCut
//                      << std::endl;
            threads[i].points = {
                    {l, u, TASK_LAYER},
                    {r, u, TASK_LAYER},
                    {l, d, TASK_LAYER},
                    {r, d, TASK_LAYER},
            };
            auto ThreadPoolRect = CrpGameObject::makeRectangle(crpDevice,
                                                               threads[i].points, true, {0, 0, .5f});
            manager->gameObjects.emplace(ThreadPoolRect.getId(), std::move(ThreadPoolRect));
        }
    }
}

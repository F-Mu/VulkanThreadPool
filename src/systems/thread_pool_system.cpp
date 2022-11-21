#include "thread_pool_system.hpp"
#include <iostream>
#include <map>

namespace crp {
    ThreadPoolSystem::ThreadPoolSystem(CrpDevice &crpDevice, const std::shared_ptr<GameObjectManager> &manager) {
        auto ThreadPoolRect = CrpGameObject::makeRectangle(crpDevice,
                                                           x, y, z, w, false);
        id = ThreadPoolRect.getId();
        manager->gameObjects.emplace(ThreadPoolRect.getId(), std::move(ThreadPoolRect));
        float mid = (left + right) / 2;
        points.resize(THREAD_NUM);
        threads.resize(THREAD_NUM);
        float hCut = (down - up) / float(THREAD_NUM);
        float yFirst = (up + hCut + up) / 2;
        float threadWidth = right - left - .1f;
        float threadHeight = (hCut - .1f);
        for (int i = 0; i < THREAD_NUM; ++i) {
            points[i] = {mid, yFirst + hCut * i, THREAD_LAYER};
            threads[i].center = points[i];
            threads[i].points.resize(4);
            float l = -threadWidth / 2, r = threadWidth / 2,
                    u = -threadHeight / 2, d = threadHeight / 2;
//            std::cout << l << ' ' << r << ' ' << u << ' ' << d << ' ' << threadUp << ' ' << threadHeight << ' ' << hCut
//                      << std::endl;
            threads[i].points = {
                    {l, u, THREAD_LAYER},
                    {r, u, THREAD_LAYER},
                    {l, d, THREAD_LAYER},
                    {r, d, THREAD_LAYER},
            };
            auto ThreadRect = CrpGameObject::makeRectangle(crpDevice,
                                                           threads[i].points, points[i], true, {0, 0, .5f});
            threads[i].id = ThreadRect.getId();
            manager->gameObjects.emplace(ThreadRect.getId(), std::move(ThreadRect));
        }
    }

    void ThreadPoolSystem::tick(FrameInfo &frameInfo) {
        for (auto it = moveTasks.begin(); it != moveTasks.end();) {
            auto &moveTask = *it;
            for (auto &kv: frameInfo.gameObjects) {
                if (kv.first == moveTask.rectangle.id) {
                    moveTask.tick();
                    kv.second.transform.translation += moveTask.speed;
//                    std::cout<<kv.second.transform.translation[2]<<std::endl;
                    break;
                }
//            assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");
            }
            if (it->isFinished())
                it = moveTasks.erase(it);
            else
                ++it;
        }
    }


    void ThreadPoolSystem::addMoveTask(Rectangle &thread, glm::vec3 &point) {
        moveTasks.emplace_back(thread, point);
    }
}

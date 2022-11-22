#include "thread_pool_system.hpp"
#include "../global/global_context.hpp"
#include "runtime_system.hpp"
#include <iostream>
#include <map>

namespace crp {
    ThreadPoolSystem::ThreadPoolSystem(CrpDevice &crpDevice, const std::shared_ptr<GameObjectManager> &manager)
            : stop{false} {
        auto ThreadPoolRect = CrpGameObject::makeRectangle(crpDevice,
                                                           x, y, z, w, false);
        id = ThreadPoolRect.getId();
        manager->gameObjects.emplace(ThreadPoolRect.getId(), std::move(ThreadPoolRect));
        points.resize(THREAD_NUM);
        threadPoolInit(crpDevice, manager);
    }

    void ThreadPoolSystem::threadPoolInit(CrpDevice &crpDevice, const std::shared_ptr<GameObjectManager> &manager) {
        //thread_pool
        threads.resize(THREAD_NUM);
        float mid = (left + right) / 2;
        float hCut = (down - up) / float(THREAD_NUM);
        float yFirst = (up + hCut + up) / 2;
        float threadWidth = right - left - .1f;
        float threadHeight = (hCut - .1f);
        for (int i = 0; i < THREAD_NUM; ++i) {
            points[i] = {mid, yFirst + hCut * i, THREAD_LAYER};
            threads[i].rectangle.center = points[i];
            threads[i].rectangle.points.resize(4);
            float l = -threadWidth / 2, r = threadWidth / 2,
                    u = -threadHeight / 2, d = threadHeight / 2;
            threads[i].rectangle.points = {
                    {l, u, THREAD_LAYER},
                    {r, u, THREAD_LAYER},
                    {l, d, THREAD_LAYER},
                    {r, d, THREAD_LAYER},
            };

//            PRINT(threads[i].rectangle.center);
            threads[i].th = std::thread([this, i] {
                while (!this->stop) {
                    task_type task;
//                    std::cout<<this->threads[i].rectangle.points[0][0]<<std::endl;
                    {
                        std::unique_lock<std::mutex> lock(this->_mut);
                        this->start.wait(lock, [this] {
                            return this->stop || !tasks.empty();
                        });
                        if (stop)return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    tasks.front();
//                    std::cout<<"#"<<std::endl;
//                    std::cout<<tasks.size()<<std::endl;
//                    std::cout<<i<<std::endl;
//                    PRINT(threads[i].rectangle.center);
                    addMoveTask(threads[i].rectangle, globalContext.runTimeSystem->points[i]);

//                    while (!stop && threads[i].rectangle.center != globalContext.runTimeSystem->points[i]) {
////                        std::cout << "#" << std::endl;
////                        PRINT(threads[i].rectangle.center);
////                        PRINT(globalContext.runTimeSystem->points[i]);
////                        std::cout << "#" << std::endl;
//                        continue;
//                    }
                    {
//                        std::cout << "#" << std::endl;
                        std::unique_lock<std::mutex> lock(runMut);
                        this->run.wait(lock, [this, i] {
//                            return true;
                            return this->stop || threads[i].rectangle.center == globalContext.runTimeSystem->points[i];
                        });
                        if (stop)return;
                    }
                    task();
                    addMoveTask(threads[i].rectangle, points[i]);
                    {
                        std::unique_lock<std::mutex> lock(this->resetMut);
                        this->reset.wait(lock, [this, i] {
                            return this->stop || threads[i].rectangle.center == points[i];
                        });
                        if (stop)return;
                    }
                }
            });
            auto ThreadRect = CrpGameObject::makeRectangle(crpDevice,
                                                           threads[i].rectangle.points, points[i], true, {0, 0, .5f});
            threads[i].rectangle.id = ThreadRect.getId();
            manager->gameObjects.emplace(ThreadRect.getId(), std::move(ThreadRect));
        }
    }


    void ThreadPoolSystem::tick(FrameInfo &frameInfo) {
        for (auto it = moveTasks.begin(); it != moveTasks.end();) {
            auto moveTask = it->get();
            for (auto &kv: frameInfo.gameObjects) {
                if (kv.first == moveTask->rectangle.id) {
                    moveTask->tick();
                    kv.second.transform.translation = moveTask->rectangle.center;
//                    PRINT(kv.second.transform.translation);
//                    PRINT(moveTask.direction);
                    break;
                }
//            assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");
            }
            if (moveTask->isFinished())
                it = moveTasks.erase(it);
            else
                ++it;
        }
        run.notify_all();
    }

    void ThreadPoolSystem::roundTick() {
        //order
        reset.notify_all();
    }

    void ThreadPoolSystem::addMoveTask(Rectangle &thread, glm::vec3 &point) {
        moveTasks.emplace_back(std::make_unique<TaskToMove>(thread, point));
    }

    //thread_pool
    void ThreadPoolSystem::clear() {
        stop = true;
        start.notify_all();
        reset.notify_all();
        run.notify_all();
        for (auto &thread: threads) {
            if (thread.th.joinable())
                thread.th.join();
        }
    }

}

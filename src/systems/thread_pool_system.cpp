#include "thread_pool_system.hpp"
#include "../global/global_context.hpp"
#include "../resources/game_object_manager.hpp"
#include "../resources/move_task_manager.hpp"
#include "runtime_system.hpp"
#include "task_queue_system.hpp"
#include <iostream>
#include <map>

namespace crp {
    ThreadPoolSystem::ThreadPoolSystem(CrpDevice &crpDevice)
            : stop{false}, crpDevice{crpDevice} {
        auto ThreadPoolRect = CrpGameObject::makeRectangle(crpDevice,
                                                           x, y, z, w, false);
        id = ThreadPoolRect.getId();
        globalContext.gameObjectManager->gameObjects.emplace(ThreadPoolRect.getId(), std::move(ThreadPoolRect));
        points.resize(THREAD_NUM);
        threadPoolInit();
    }

    void ThreadPoolSystem::threadPoolInit() {
        //thread_pool
        threads.resize(THREAD_NUM);
        float mid = (left + right) / 2;
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
            threads[i].points = {
                    {l, u, THREAD_LAYER},
                    {r, u, THREAD_LAYER},
                    {l, d, THREAD_LAYER},
                    {r, d, THREAD_LAYER},
            };
            auto ThreadRect = CrpGameObject::makeRectangle(crpDevice,
                                                           threads[i].points, points[i], true, {0, 0, .5f});
            threads[i].id = ThreadRect.getId();
            globalContext.gameObjectManager->gameObjects.emplace(ThreadRect.getId(), std::move(ThreadRect));
        }
    }


    void ThreadPoolSystem::threadsInit(std::list<Task> &tasks) {
        for (int i = 0; i < THREAD_NUM; ++i) {
            threads[i].th = std::thread([this, i, &tasks] {
                while (!this->stop) {
                    Task task;
                    {
                        std::unique_lock<std::mutex> lock(this->startMut);
                        this->start.wait(lock, [this, &tasks] {
                            if (!globalContext.runTimeSystem)return false;
                            return this->stop ||
                                   globalContext.runTimeSystem->taskQueueSystem->isSorted();
                        });
//                        std::cout<<"#"<<std::endl;
                        if (stop)return;
                        task = std::move(tasks.front());
                        tasks.pop_front();
                        globalContext.runTimeSystem->taskQueueSystem->sortTasks();
                        --availableThreadNum;
                    }
                    task.run(globalContext.runTimeSystem->points[i]);
                    globalContext.moveTaskManager->addMoveTask(threads[i], globalContext.runTimeSystem->points[i]);
                    {
                        std::unique_lock<std::mutex> lock(runMut);
                        this->run.wait(lock, [this, i, &task] {
                            return this->stop ||
                                   (STRICT_EQUAL(threads[i].center, globalContext.runTimeSystem->points[i])
                                    && STRICT_EQUAL(task.center, threads[i].center));
                        });
                        if (stop)return;
                    }
                    task.task();
                    globalContext.runTimeSystem->taskQueueSystem->addDeleteTask(task);
//                    std::cout << "#" << std::endl;
                    globalContext.moveTaskManager->addMoveTask(threads[i], points[i]);
                    {
                        std::unique_lock<std::mutex> lock(this->resetMut);
                        this->reset.wait(lock, [this, i] {
                            return this->stop || threads[i].center == points[i];
                        });
                        if (stop)return;
                    }
//                    std::cout << "#" << std::endl;
                    {
                        std::unique_lock<std::mutex> lock(this->startMut);
                        ++availableThreadNum;
                    }
                }
            });
        }
        availableThreadNum = THREAD_NUM;
    }

    void ThreadPoolSystem::tick(FrameInfo &frameInfo) {
        run.notify_all();
        reset.notify_all();
    }

    void ThreadPoolSystem::roundTick() {
        //order
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

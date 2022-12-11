#include "thread_pool_system.hpp"
#include "function/global/global_context.hpp"
#include "resources/manager/game_object_manager.hpp"
#include "function/framework/component/render_component.hpp"
#include "resources/res_type/thread.hpp"
#include <iostream>
#include <map>

namespace crp {
    ThreadPoolSystem::ThreadPoolSystem() :
            Rectangle(Rectangle::MakeRectangle({x, y, w, z})),
            stop{false} {
        points.resize(THREAD_NUM);
        threadPoolInit();
    }

    void ThreadPoolSystem::threadPoolInit() {
        //thread_pool
        float mid = (left + right) / 2;
        float hCut = (down - up) / float(THREAD_NUM);
        float yFirst = (up + hCut + up) / 2;
        float threadWidth = right - left - .05f;
        float threadHeight = (hCut - .05f);
        float l = -threadWidth / 2, r = threadWidth / 2,
                u = -threadHeight / 2, d = threadHeight / 2;
        std::vector<glm::vec3> meshPoints = {
                {r, u, 0},
                {l, u, 0},
                {r, d, 0},
                {l, d, 0},
        };
        for (int i = 0; i < THREAD_NUM; ++i) {
            threads.emplace_back(Thread(meshPoints, THREAD_COLOR, true, true));
            points[i] = {mid, yFirst + hCut * i, THREAD_LAYER};

            threads[i].setPosition(points[i]);
        }
    }


    void ThreadPoolSystem::threadsInit(std::list<Task> &tasks) {
        for (int i = 0; i < THREAD_NUM; ++i) {
            threads[i].th = std::thread([this, i, &tasks] {
                while (!this->stop) {
                    std::shared_ptr<Task> task;
                    {
                        std::unique_lock<std::mutex> lock(this->startMut);
                        this->start.wait(lock, [this] {
                            if (!globalContext.runTimeSystem)return false;
                            return this->stop ||
                                   globalContext.runTimeSystem->taskQueueSystem->isSorted();
                        });
                        if (stop)return;
                        {
                            std::lock_guard<std::mutex> taskMut(globalContext.runTimeSystem->taskQueueSystem->taskMut);
                            task = std::make_shared<Task>(std::move(tasks.front()));
                            task->run(globalContext.runTimeSystem->points[i]);
                            tasks.pop_front();
                        }
                        --availableThreadNum;
                    }
                    threads[i].Move(globalContext.runTimeSystem->points[i]);
                    {
                        std::unique_lock<std::mutex> lock(runMut);
                        this->run.wait(lock, [this, i, &task] {
                            return this->stop ||
                                   (STRICT_EQUAL(threads[i].getCenter(), globalContext.runTimeSystem->points[i])
                                    && STRICT_EQUAL(task->getCenter(), threads[i].getCenter()));
                        });
                        if (stop)return;
                    }
                    task->task();
                    *task->ready = true;
                    task->setDelete();
                    threads[i].Move(points[i]);
                    {
                        std::unique_lock<std::mutex> lock(this->resetMut);
                        this->reset.wait(lock, [this, i] {
                            return this->stop || threads[i].getCenter() == points[i];
                        });
                        if (stop)return;
                    }
                    {
                        std::unique_lock<std::mutex> lock(this->startMut);
                        ++availableThreadNum;
                    }
                }
            });
        }
        availableThreadNum = THREAD_NUM;
    }

    void ThreadPoolSystem::tick() {
        run.notify_all();
        reset.notify_all();
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

#include "thread_pool_system.hpp"
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
            threads[i].th = std::thread([this] {
                while (!this->stop) {
                    task_type task;
                    {
                        std::unique_lock<std::mutex> lock(this->_mut);
                        this->cond.wait(lock, [this] {
                            return this->stop || !tasks.empty();
                        });
                        if (stop)return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
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
            auto ThreadRect = CrpGameObject::makeRectangle(crpDevice,
                                                           threads[i].rectangle.points, points[i], true, {0, 0, .5f});
            threads[i].rectangle.id = ThreadRect.getId();
            manager->gameObjects.emplace(ThreadRect.getId(), std::move(ThreadRect));
        }
    }


    void ThreadPoolSystem::tick(FrameInfo &frameInfo) {
        for (auto it = moveTasks.begin(); it != moveTasks.end();) {
            auto &moveTask = *it;
            for (auto &kv: frameInfo.gameObjects) {
                if (kv.first == moveTask.rectangle.id) {
                    moveTask.tick();
                    kv.second.transform.translation = moveTask.rectangle.center;
//                    PRINT(kv.second.transform.translation);
//                    PRINT(moveTask.direction);
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

    //thread_pool
    void ThreadPoolSystem::clear() {
        stop = true;
        cond.notify_all();
        for (auto &thread: threads) {
            if (thread.th.joinable())
                thread.th.join();
        }
    }

    template<typename Fun, typename... Args>
    std::future<typename std::invoke_result<Fun, Args...>::type>
    ThreadPoolSystem::add(Fun &&fun, Args &&...args) {
        using result_type = typename std::invoke_result<Fun, Args...>::type;
        using task = std::packaged_task<result_type()>;
        auto t = std::make_shared<task>(std::bind(std::forward<Fun>(fun), std::forward<Args>(args)...));
        auto ret = t->get_future();
        {
            std::lock_guard<std::mutex> lock(this->_mut);
            if (stop)
                throw std::runtime_error("Thread pool has stopped");
            tasks.emplace([t] { (*t)(); });
        }
        cond.notify_one();
        return ret;
    }

}

#pragma once

#include <vector>
#include <future>
#include <queue>
#include <thread>
#include <glm/glm.hpp>
#include "../crp_game_obejct.hpp"
#include "../core/macro.hpp"
#include "../resources/game_object_manager.hpp"
#include "../crp_frame_info.hpp"
#include "../core/rectangle.hpp"

namespace crp {

    class Thread {
    public:
        std::thread th;
        Rectangle rectangle;
    };

    class ThreadPoolSystem {
    public:
        ThreadPoolSystem(CrpDevice &crpDevice, const std::shared_ptr<GameObjectManager> &manager);

        void clear();

        void addMoveTask(Rectangle &thread, glm::vec3 &point);

        void tick(FrameInfo &frameInfo);

        void roundTick();

        void threadPoolInit(CrpDevice &crpDevice, const std::shared_ptr<GameObjectManager> &manager);

        std::vector<glm::vec3> points;
        std::vector<Thread> threads;
        std::vector<std::unique_ptr<TaskToMove>> moveTasks;


//        template<typename Fun, typename ...Args>
//        std::future<typename std::invoke_result<Fun, Args...>::type> add(Fun &&, Args &&...);

    private:
        float up = -1.1f, down = 1.1f, left = -1.5f, right = -1.0f;
        glm::vec3 x{left, up, QUEUE_LAYER}, y{right, up, QUEUE_LAYER}, z{left, down, QUEUE_LAYER}, w{right, down,
                                                                                                     QUEUE_LAYER};
        GameObjectManager::id_t id;

        //thread_pool
        using task_type = std::function<void()>;
        std::mutex _mut;
        std::mutex resetMut;
        std::mutex runMut;
        std::queue<task_type> tasks;
        std::atomic<bool> stop;
        std::condition_variable start;
        std::condition_variable reset;
        std::condition_variable run;

    public:
        //thread_pool
        template<typename Fun, typename... Args>
        std::future<typename std::invoke_result<Fun, Args...>::type>
        add(Fun &&fun, Args &&...args) {
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
            start.notify_one();
            return ret;
        }
    };
}
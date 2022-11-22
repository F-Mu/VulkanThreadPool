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

        void threadPoolInit(CrpDevice &crpDevice, const std::shared_ptr<GameObjectManager> &manager);

        std::vector<glm::vec3> points;
        std::vector<Thread> threads;
        std::vector<TaskToMove> moveTasks;

        //thread_pool
        template<typename Fun, typename ...Args>
        std::future<typename std::invoke_result<Fun, Args...>::type> add(Fun &&, Args &&...);

    private:
        float up = -1.1f, down = 1.1f, left = -1.5f, right = -1.0f;
        glm::vec3 x{left, up, QUEUE_LAYER}, y{right, up, QUEUE_LAYER}, z{left, down, QUEUE_LAYER}, w{right, down,
                                                                                                     QUEUE_LAYER};
        GameObjectManager::id_t id;

        //thread_pool
        using task_type = std::function<void()>;
        std::mutex _mut;
        std::queue<task_type> tasks;
        std::atomic<bool> stop;
        std::condition_variable cond;
    };
}
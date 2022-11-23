#pragma once

#include <vector>
#include <future>
#include <queue>
#include <thread>
#include <list>
#include <glm/glm.hpp>
#include "../crp_game_obejct.hpp"
#include "../core/macro.hpp"
#include "../resources/game_object_manager.hpp"
#include "../crp_frame_info.hpp"
#include "../core/rectangle.hpp"

namespace crp {

    class Thread : public Rectangle {
    public:
        std::thread th;
    };

    class RuntimeSystem;

    class Task;

    class MoveTaskManager;

    class ThreadPoolSystem {
    public:
        friend RuntimeSystem;

        ThreadPoolSystem(CrpDevice &crpDevice);

        void clear();

        void tick(FrameInfo &frameInfo);

        void threadPoolInit();

        void threadsInit(std::list<Task> &tasks);

        int availableNum() { return availableThreadNum; }

        std::vector<glm::vec3> points;
        std::vector<Thread> threads;

        //thread_pool
        std::mutex startMut;
        std::mutex resetMut;
        std::mutex runMut;
//        std::queue<task_type> tasks;

        std::atomic<bool> stop;
        std::condition_variable start;
        std::condition_variable reset;
        std::condition_variable run;
    private:
        float up = -1.1f, down = 1.1f, left = -1.5f, right = -1.0f;
        glm::vec3 x{left, up, QUEUE_LAYER}, y{right, up, QUEUE_LAYER}, z{left, down, QUEUE_LAYER}, w{right, down,
                                                                                                     QUEUE_LAYER};
        GameObjectManager::id_t id;
        CrpDevice &crpDevice;
        int availableThreadNum;
    public:

    };
}
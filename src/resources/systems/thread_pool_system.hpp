#pragma once

#include <utility>
#include <vector>
#include <future>
#include <queue>
#include <thread>
#include <list>
#include <glm/glm.hpp>
#include "function/framework/crp_game_obejct.hpp"
#include "core/macro.hpp"
#include "resources/manager/game_object_manager.hpp"
#include "render/crp_frame_info.hpp"
#include "function/framework/rectangle.hpp"

namespace crp {

    class Thread : public Rectangle {
    public:
        Thread(CrpDevice &device, std::vector<glm::vec3> points, glm::vec3 color = {1., 1., 1.}, bool fill = false,
               bool moveAble = false) :
                Rectangle(std::move(Rectangle::MakeRectangle(std::move(points), color, fill, moveAble))) {};
        std::thread th;
    };

    class RuntimeSystem;

    class Task;

    class ThreadPoolSystem : public Rectangle {
    public:
        friend RuntimeSystem;

        ThreadPoolSystem(CrpDevice &crpDevice);

        void clear();

        void tick();

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
//        static constexpr float up = -1.1f, down = 1.1f, left = -1.5f, right = -1.0f;
        static constexpr float up = -0.55f, down = 0.55f, left = -0.75f, right = -0.5f;
        static constexpr glm::vec3 x{left, up, QUEUE_LAYER}, y{right, up, QUEUE_LAYER}, z{left, down, QUEUE_LAYER}, w{
                right, down, QUEUE_LAYER};
        CrpDevice &crpDevice;
        int availableThreadNum;
    public:

    };
}
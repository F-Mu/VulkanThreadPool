#pragma once

#include <glm/glm.hpp>
#include "function/framework/game_object.hpp"
#include "core/macro.hpp"
#include "resources/manager/game_object_manager.hpp"
#include "render/render_frame_info.hpp"
#include "function/framework/rectangle.hpp"
#include "resources/res_type/thread.hpp"

namespace crp {
    class RuntimeSystem;

    class Task;

    class ThreadPoolSystem : public Rectangle {
    public:
        ThreadPoolSystem();

        void clear();

        void tick();

        void threadPoolInit();

        void threadsInit(std::list<Task> &tasks);

        int availableNum() const { return availableThreadNum; }

        std::vector<glm::vec3> points;
        std::vector<Thread> threads;

        //thread_pool
        std::mutex startMut;
        std::mutex resetMut;
        std::mutex runMut;
        std::atomic<bool> stop;
        std::condition_variable start;
        std::condition_variable reset;
        std::condition_variable run;
    private:
        static constexpr float up = -0.55f, down = 0.55f, left = -0.75f, right = -0.5f;
        static constexpr glm::vec3 x{left, up, QUEUE_LAYER}, y{right, up, QUEUE_LAYER}, z{left, down, QUEUE_LAYER}, w{
                right, down, QUEUE_LAYER};
        int availableThreadNum{};
    };
}
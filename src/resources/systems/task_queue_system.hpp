#pragma once

#include "render/render_device.hpp"
#include "core/macro.hpp"
#include "function/framework/rectangle.hpp"
#include "render/render_frame_info.hpp"
#include "function/global/global_context.hpp"
#include "resources/res_type/task.hpp"
namespace crp {
    class TaskQueueSystem : public Rectangle {
    public:
        explicit TaskQueueSystem();

        void tick();

        void sortTasks();

        void lock();

        void unlock();

        bool isSorted();

        bool isTaskInQueue(Task &task) {
            return fabs(task.getCenter().x - points[0].x) < STRICT_EPS;
        }

        std::vector<glm::vec3> points;
        std::list<Task> tasks;
        std::mutex taskMut;
    private:
        static constexpr float up = -0.55f, down = 0.55f, left = 0.5f, right = 0.75f;
        static constexpr glm::vec3 x{left, up, QUEUE_LAYER}, y{right, up, QUEUE_LAYER}, z{left, down, QUEUE_LAYER}, w{
                right, down,
                QUEUE_LAYER};
        bool locked{};

    public:
        //thread_pool
        template<typename Fun, typename... Args>
        std::optional<TaskResult<Fun, Args...>>
        add(Fun &&fun, Args &&...args) {
            using result_type = typename std::invoke_result<Fun, Args...>::type;
            using task = std::packaged_task<result_type()>;

            if (locked)
                return std::optional<TaskResult<Fun, Args...>>();

            auto t = std::make_shared<task>(std::bind(std::forward<Fun>(fun), std::forward<Args>(args)...));
            auto ret = t->get_future();
            {
                std::lock_guard<std::mutex> lock(this->taskMut);
                tasks.emplace_back([t] { (*t)(); });
                return TaskResult<Fun, Args...>(ret, tasks.back().ready, locked);
            }
        }
    };
}

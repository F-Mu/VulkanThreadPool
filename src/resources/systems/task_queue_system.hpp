#pragma once

#include <glm/glm.hpp>
#include <queue>
#include <list>
#include <future>
#include <utility>
#include "render/crp_device.hpp"
#include "core/macro.hpp"
#include "function/framework/rectangle.hpp"
#include "render/crp_frame_info.hpp"
#include "function/global/global_context.hpp"

namespace crp {
    template<typename Fun, typename... Args>
    class TaskResult {
    public:
        TaskResult(std::future<typename std::invoke_result<Fun, Args...>::type> &result,
                   std::shared_ptr<bool> &r, bool &lock)
                : result{std::move(result)}, ready{r}, lockTaskQueue{lock} {
        }

        std::shared_ptr<bool> ready;
        bool &lockTaskQueue;
        bool finished;

        std::future<typename std::invoke_result<Fun, Args...>::type> result;

        std::optional<typename std::invoke_result<Fun, Args...>::type> getResult(bool shouldWait) {
            if (!*ready && shouldWait)
                lockTaskQueue = true;
            if (finished)return std::optional<typename std::invoke_result<Fun, Args...>::type>();
            if (*ready) {
                lockTaskQueue = false;
                finished = true;
                return result.get();
            }
            return std::optional<typename std::invoke_result<Fun, Args...>::type>();
        }
    };


    class Task : public Rectangle {
    public:
        Task(std::function<void()> fun) :
                task{std::move(fun)}, Rectangle{Rectangle::MakeRectangle(meshPoints, TASK_COLOR, true, true)} {
            ready = std::make_shared<bool>(false);
            setPosition(initPosition);
        };

        std::function<void()> task;

        std::shared_ptr<bool> ready{};

        void run(glm::vec3 &point);

        static glm::vec3 initPosition;
        static float taskWidth, taskHeight;

        static float l, r, u, d;
        static std::vector<glm::vec3> meshPoints;
    };

    class TaskQueueSystem : public Rectangle {
    public:
        explicit TaskQueueSystem();

        void tick(FrameInfo &frameInfo);

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
//            globalContext.gameObjectManager->gameObjects.emplace(rec.getId(),
//                                                                 rec.gameObject->shared_from_this());
            auto ret = t->get_future();
            {
                std::lock_guard<std::mutex> lock(this->taskMut);
                tasks.emplace_back([t] { (*t)(); });
                return TaskResult<Fun, Args...>(ret, tasks.back().ready, locked);
            }
        }
    };
}

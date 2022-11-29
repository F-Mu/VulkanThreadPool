#pragma once

#include <glm/glm.hpp>
#include <queue>
#include <list>
#include <future>
#include <utility>
#include "render/crp_device.hpp"
#include "core/macro.hpp"
#include "core/rectangle.hpp"
#include "render/crp_frame_info.hpp"
#include "function/global/global_context.hpp"
#include "resources/manager/move_task_manager.hpp"
#include "runtime_system.hpp"

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
        Task(std::function<void()> fun, Rectangle &rec) : task{std::move(fun)}, Rectangle{rec} {
            ready = std::make_shared<bool>(false);
        };

        Task() {};
        std::function<void()> task;

        std::shared_ptr<bool> ready{};

        void run(glm::vec3 &point);
    };

    class TaskQueueSystem {
    public:
        explicit TaskQueueSystem(CrpDevice &crpDevice);

        void addDeleteTask(Rectangle &task);

        void tick(FrameInfo &frameInfo);

        void sortTasks();

        void lock();

        void unlock();

        bool isSorted();

        bool isTaskInQueue(Task &task) {
            return fabs(task.center.x - points[0].x) < STRICT_EPS;
        }

        std::vector<glm::vec3> points;
        std::list<Task> tasks;
        std::list<std::unique_ptr<std::pair<GameObjectManager::id_t, float>>> deleteTasks;
        std::vector<GameObjectManager::id_t> shouldDelete;
        std::mutex taskMut;
    private:
        float up = -1.1f, down = 1.1f, left = 1.f, right = 1.5f;
        glm::vec3 x{left, up, QUEUE_LAYER}, y{right, up, QUEUE_LAYER}, z{left, down, QUEUE_LAYER}, w{right, down,
                                                                                                     QUEUE_LAYER};
        glm::vec3 taskInitPosition;
        float taskWidth, taskHeight;
        GameObjectManager::id_t id;
        CrpDevice &crpDevice;
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
            Rectangle rec;
            rec.movable = true;
            rec.center = taskInitPosition;
            rec.points.resize(4);
            float l = -taskWidth / 2, r = taskWidth / 2,
                    u = -taskHeight / 2, d = taskHeight / 2;
            rec.points = {
                    {l, u, 0},
                    {r, u, 0},
                    {l, d, 0},
                    {r, d, 0},
            };
            auto TaskRect = CrpGameObject::makeRectangle(crpDevice, rec.points,
                                                         {rec.center.x, rec.center.y, TASK_LAYER}, true, {0, .5f, .5f});
            rec.id = TaskRect.getId();
            globalContext.gameObjectManager->gameObjects.emplace(TaskRect.getId(), std::move(TaskRect));
            auto t = std::make_shared<task>(std::bind(std::forward<Fun>(fun), std::forward<Args>(args)...));
            auto ret = t->get_future();

            {
                std::lock_guard<std::mutex> lock(this->taskMut);
                tasks.emplace_back([t] { (*t)(); }, rec);
                return TaskResult<Fun, Args...>(ret, tasks.back().ready, locked);
            }
        }
    };
}

#pragma once

#include <glm/glm.hpp>
#include <queue>
#include <list>
#include <future>
#include <utility>
#include "../crp_device.hpp"
#include "../core/macro.hpp"
#include "../core/rectangle.hpp"
#include "../crp_frame_info.hpp"
#include "../global/global_context.hpp"
#include "../resources/move_task_manager.hpp"

namespace crp {
    class Thread;

    class Task : public Rectangle {
    public:
        Task(std::function<void()> fun, Rectangle &rec) : task{std::move(fun)}, Rectangle(rec) {};

        Task() {};
        std::function<void()> task;

        void run(glm::vec3 &point);
    };

    class TaskQueueSystem {
    public:
        TaskQueueSystem(CrpDevice &crpDevice);

        void addDeleteTask(Rectangle &task);

        void tick(FrameInfo &frameInfo);

        void sortTasks();

        bool isSorted();

        std::vector<glm::vec3> points;
        std::list<Task> tasksInQueue;
        std::queue<Task> tasksRun;
        std::queue<Task> tasksWait;
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
    public:
        //thread_pool
        template<typename Fun, typename... Args>
        std::future<typename std::invoke_result<Fun, Args...>::type>
        add(Fun &&fun, Args &&...args) {
//            std::cout<<tasksWait.size()<<std::endl;
//            if (tasksWait.size() == MAX_TASK_NUM)
//                return nullptr;
            using result_type = typename std::invoke_result<Fun, Args...>::type;
            using task = std::packaged_task<result_type()>;

            Rectangle rec;
            rec.movable = true;
            rec.center = taskInitPosition;
            rec.points.resize(4);
            float l = -taskWidth / 2, r = taskWidth / 2,
                    u = -taskHeight / 2, d = taskHeight / 2;
            rec.points = {
                    {l, u, TASK_LAYER},
                    {r, u, TASK_LAYER},
                    {l, d, TASK_LAYER},
                    {r, d, TASK_LAYER},
            };
            auto TaskRect = CrpGameObject::makeRectangle(crpDevice, rec.points, rec.center, true, {0, .5f, .5f});
            rec.id = TaskRect.getId();
            globalContext.gameObjectManager->gameObjects.emplace(TaskRect.getId(), std::move(TaskRect));
            auto t = std::make_shared<task>(std::bind(std::forward<Fun>(fun), std::forward<Args>(args)...));
            auto ret = t->get_future();

            tasksWait.emplace([t] { (*t)(); }, rec);
            return ret;
        }
    };
}

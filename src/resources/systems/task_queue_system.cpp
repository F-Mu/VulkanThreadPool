#include "task_queue_system.hpp"

namespace crp {
    glm::vec3 Task::initPosition = {};
    float Task::taskHeight = 0;
    float Task::taskWidth = 0;
    float Task::l = 0, Task::r = 0, Task::u = 0, Task::d = 0;
    std::vector<glm::vec3>Task::meshPoints{};

    TaskQueueSystem::TaskQueueSystem() :
            Rectangle{std::move(Rectangle::MakeRectangle({x, y, w, z}))} {
        float mid = (left + right) / 2;
        points.resize(TASK_NUM);
        float hCut = (down - up) / float(TASK_NUM);
        float yFirst = (up + hCut + up) / 2;

        //initTask
        Task::taskWidth = right - left - .075f;
        Task::taskHeight = (hCut - .05f);

        Task::l = -Task::taskWidth / 2;
        Task::r = Task::taskWidth / 2;
        Task::u = -Task::taskHeight / 2;
        Task::d = Task::taskHeight / 2;
        for (int i = 0; i < TASK_NUM; ++i) {
            points[i] = {mid, yFirst + hCut * i, TASK_LAYER};
        }
        Task::initPosition = points.back();
        Task::initPosition.x += 1;
        Task::meshPoints = {
                {Task::l, Task::u, 0},
                {Task::r, Task::u, 0},
                {Task::l, Task::d, 0},
                {Task::r, Task::d, 0},
        };
    }

    void TaskQueueSystem::tick() {
        sortTasks();
    }

    void TaskQueueSystem::sortTasks() {
        std::lock_guard<std::mutex> lock(this->taskMut);
        int cnt = -1;
        for (auto &i: tasks) {
            ++cnt;
            if (cnt == TASK_NUM)return;
            if (STRICT_EQUAL(i.getCenter(), points[cnt]))continue;
            if (isTaskInQueue(i))
                i.Move(points[cnt], FRAME_TIME / 5);
            else {
                std::vector<glm::vec3> target = {points[TASK_NUM - 1], points[cnt]};
                i.Move(target);
            }
        }
    }

    bool TaskQueueSystem::isSorted() {
        if (tasks.empty())return false;
        return !tasks.front().isMove();
    }

    void TaskQueueSystem::lock() {
        locked = true;
    }

    void TaskQueueSystem::unlock() {
        locked = false;
    }

    void Task::run(glm::vec3 &point) {
        std::vector<glm::vec3> destinations;
        auto center = getCenter();
        destinations.emplace_back(MID(center[0], point[0]), center[1], TASK_LAYER);
        destinations.emplace_back(MID(center[0], point[0]), point[1], TASK_LAYER);
        destinations.emplace_back(point[0], point[1], TASK_LAYER);
        Move(destinations);
    }

}

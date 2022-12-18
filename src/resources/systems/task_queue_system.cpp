#include "task_queue_system.hpp"

namespace crp {
    TaskQueueSystem::TaskQueueSystem() :
            Rectangle{Rectangle::MakeRectangle({x, y, w, z})} {
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
                {Task::r, Task::u, 0},
                {Task::l, Task::u, 0},
                {Task::r, Task::d, 0},
                {Task::l, Task::d, 0},
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
        std::lock_guard<std::mutex> lock(this->taskMut);
        if (tasks.empty())return false;
        return (!tasks.front().isMove()) && (EQUAL(tasks.front().getCenter(), points[0]));
    }

    void TaskQueueSystem::lock() {
        locked = true;
    }

    void TaskQueueSystem::unlock() {
        locked = false;
    }

    void TaskQueueSystem::addSleepWork(long long time) {
        if (locked)return;
        {
            std::lock_guard<std::mutex> lock(this->taskMut);
            tasks.emplace_back(Task::makeSleepTask(time));
        }
    }
}

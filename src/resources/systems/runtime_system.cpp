#include "runtime_system.hpp"
#include "task_queue_system.hpp"
#include "thread_pool_system.hpp"
#include "resources/manager/game_object_manager.hpp"

namespace crp {
    RuntimeSystem::RuntimeSystem() :
            Rectangle{Rectangle::MakeRectangle({x, y, w, z})},
            taskQueueSystem{std::make_shared<TaskQueueSystem>()},
            threadPoolSystem{std::make_shared<ThreadPoolSystem>()} {
        float mid = (left + right) / 2;
        points.resize(THREAD_NUM);
        float hCut = (down - up) / float(THREAD_NUM);
        float yFirst = (up + hCut + up) / 2;
        for (int i = 0; i < THREAD_NUM; ++i) {
            points[i] = {mid, yFirst + hCut * i, THREAD_LAYER};
        }
        threadPoolSystem->threadsInit(taskQueueSystem->tasks);
    }

    void RuntimeSystem::tick() {
        taskQueueSystem->tick();
        threadPoolSystem->tick();
        if (!taskQueueSystem->isSorted())return;
        if (threadPoolSystem->availableNum() != 0)
            threadPoolSystem->start.notify_one();
    }

    void RuntimeSystem::clear() {
        threadPoolSystem->clear();
        threadPoolSystem.reset();
        taskQueueSystem.reset();
    }
}

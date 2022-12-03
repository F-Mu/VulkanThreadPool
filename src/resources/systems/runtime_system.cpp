#include "runtime_system.hpp"
#include "task_queue_system.hpp"
#include "thread_pool_system.hpp"
#include "function/global/global_context.hpp"
#include "resources/manager/game_object_manager.hpp"
#include <cmath>

namespace crp {
    RuntimeSystem::RuntimeSystem(CrpDevice &crpDevice) :
            Rectangle{std::move(Rectangle::MakeRectangle({x, y, w, z}))},
                      crpDevice{crpDevice},
                      taskQueueSystem{std::make_shared<TaskQueueSystem>()},
                      threadPoolSystem{std::make_shared<ThreadPoolSystem>(crpDevice)} {
        threadPoolSystem->threadsInit(taskQueueSystem->tasks);
//        globalContext.gameObjectManager->gameObjects[getId()] = *this;
//        globalContext.gameObjectManager->gameObjects.emplace(getId(), gameObject->shared_from_this());

        float mid = (left + right) / 2;
        points.resize(THREAD_NUM);
        float hCut = (down - up) / float(THREAD_NUM);
        float yFirst = (up + hCut + up) / 2;
        for (int i = 0; i < THREAD_NUM; ++i) {
            points[i] = {mid, yFirst + hCut * i, THREAD_LAYER};
        }
    }

    void RuntimeSystem::tick(FrameInfo &frameInfo) {
        taskQueueSystem->tick(frameInfo);
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

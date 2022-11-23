#include "task_queue_system.hpp"
#include "../first_app.hpp"
#include "../global/global_context.hpp"
#include "../resources/move_task_manager.hpp"
#include "../resources/game_object_manager.hpp"
#include "runtime_system.hpp"

namespace crp {
    TaskQueueSystem::TaskQueueSystem(CrpDevice &crpDevice) : crpDevice{crpDevice} {
        auto TaskQueueRect = CrpGameObject::makeRectangle(crpDevice,
                                                          x, y, z, w, false);
        id = TaskQueueRect.getId();
        globalContext.gameObjectManager->gameObjects.emplace(TaskQueueRect.getId(), std::move(TaskQueueRect));
        float mid = (left + right) / 2;
        points.resize(TASK_NUM);
        float hCut = (down - up) / float(TASK_NUM);
        float yFirst = (up + hCut + up) / 2;
        taskWidth = right - left - .15f;
        taskHeight = (hCut - .1f);
        for (int i = 0; i < TASK_NUM; ++i) {
            points[i] = {mid, yFirst + hCut * i, TASK_LAYER};
        }

        //init
        taskInitPosition = points.back();
        taskInitPosition.x += 1;
    }

    void TaskQueueSystem::tick(FrameInfo &frameInfo) {
        sortTasks();
        auto it = deleteTasks.begin();
        for (int i = 0; i < deleteTasks.size() && it != deleteTasks.end(); ++i) {
            auto deleteTask = it->get();
            if (!deleteTask) {
                it = deleteTasks.erase(it);
                continue;
            }
            frameInfo.gameObjects.find(deleteTask->first)
                    ->second.transform.scale -= SCALE / FRAME_TIME;
            --deleteTask->second;
            if (deleteTask->second == 0) {
                it = deleteTasks.erase(it);
            } else
                ++it;
        }
    }

    void TaskQueueSystem::sortTasks() {
        if (!tasksInQueue.empty()) {
            int cnt = -1;
            {
                std::lock_guard<std::mutex> lock(this->taskMut);
                for (auto &i: tasksInQueue) {
                    ++cnt;
                    if (i.move)continue;
                    if(i.points.size()!=4)std::cout<<"#"<<std::endl;
                    if (STRICT_EQUAL(i.center, points[cnt]))continue;
//                    std::cout<<"#"<<std::endl;
                    globalContext.moveTaskManager->addMoveTask(i, points[cnt], FRAME_TIME / 5);
                }
            }
        }

        while (tasksInQueue.size() < TASK_NUM && !tasksWait.empty()) {
            auto now = std::move(tasksWait.front());
            tasksWait.pop();
            std::vector<glm::vec3> target = {points[TASK_NUM - 1], points[tasksInQueue.size()]};
            {
                std::lock_guard<std::mutex> lock(this->taskMut);
                tasksInQueue.emplace_back(now);
            }
            globalContext.moveTaskManager->addMoveTask(tasksInQueue.back(), target);
        }
    }

    void TaskQueueSystem::addDeleteTask(Rectangle &task) {
        deleteTasks.emplace_back(std::make_unique<std::pair<GameObjectManager::id_t, float>>(task.id, FRAME_TIME));
    }

    bool TaskQueueSystem::isSorted() {
        if (tasksInQueue.empty())return false;
        return !tasksInQueue.front().move;
//        return STRICT_EQUAL(tasksInQueue.front().center, points[0]);
    }

    void Task::run(glm::vec3 &point) {
        std::vector<glm::vec3> destinations;
        destinations.emplace_back(MID(center[0], point[0]), center[1], TASK_LAYER);
        destinations.emplace_back(MID(center[0], point[0]), point[1], TASK_LAYER);
        destinations.emplace_back(point[0], point[1], TASK_LAYER);
//        std::cout<<this->move<<' '<<globalContext.runTimeSystem->taskQueueSystem->isSorted()<<std::endl;
        globalContext.moveTaskManager->addMoveTask(*this, destinations);
    }
}

#include "task_queue_system.hpp"
#include "function/global/global_context.hpp"
#include "resources/manager/move_task_manager.hpp"
#include "resources/manager/game_object_manager.hpp"
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
            auto &task = frameInfo.gameObjects.find(deleteTask->first)->second;
            auto lower = taskWidth / FRAME_TIME;
            task.rectangle->x[0] += lower;
            task.rectangle->z[0] += lower;
            task.updateRectangle(crpDevice);
//            frameInfo.gameObjects.find(deleteTask->first)
//                    ->second.transform.scale -= SCALE / FRAME_TIME;
            --deleteTask->second;
            if (deleteTask->second == 0 || task.rectangle->x[0] >= task.rectangle->y[0]) {
                globalContext.gameObjectManager->deleteById(deleteTask->first);
                it = deleteTasks.erase(it);
            } else
                ++it;
        }
    }

    void TaskQueueSystem::sortTasks() {
        std::lock_guard<std::mutex> lock(this->taskMut);
        int cnt = -1;
        for (auto &i: tasks) {
            ++cnt;
            if (cnt == TASK_NUM)return;
            if (i.move)continue;
            if (STRICT_EQUAL(i.center, points[cnt]))continue;
            if (isTaskInQueue(i))
                globalContext.moveTaskManager->addMoveTask(i, points[cnt], FRAME_TIME / 5);
            else {
                std::vector<glm::vec3> target = {points[TASK_NUM - 1], points[cnt]};
                globalContext.moveTaskManager->addMoveTask(i, target);
            }
        }
    }

    void TaskQueueSystem::addDeleteTask(Rectangle &task) {
        deleteTasks.emplace_back(std::make_unique<std::pair<GameObjectManager::id_t, float>>(task.id, FRAME_TIME));
    }

    bool TaskQueueSystem::isSorted() {
        if (tasks.empty())return false;
        return !tasks.front().move;
    }

    void TaskQueueSystem::lock() {
        locked = true;
    }

    void TaskQueueSystem::unlock() {
        locked = false;
    }

    void Task::run(glm::vec3 &point) {
        std::vector<glm::vec3> destinations;
        destinations.emplace_back(MID(center[0], point[0]), center[1], TASK_LAYER);
        destinations.emplace_back(MID(center[0], point[0]), point[1], TASK_LAYER);
        destinations.emplace_back(point[0], point[1], TASK_LAYER);
        globalContext.moveTaskManager->addMoveTask(*this, destinations);
    }
}

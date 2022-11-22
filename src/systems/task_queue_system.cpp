#include "task_queue_system.hpp"
#include "../first_app.hpp"
#include "../global/global_context.hpp"
#include "../resources/move_task_manager.hpp"
#include "../resources/game_object_manager.hpp"

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
//        std::cout<<m1oveTasks.size()<<std::endl;
        for (auto it = deleteTasks.begin(); it != deleteTasks.end();) {
            auto &deleteTask = *it;
            for (auto &kv: frameInfo.gameObjects) {
                if (kv.first == deleteTask.first) {
                    kv.second.transform.scale -= SCALE / FRAME_TIME;
                    --deleteTask.second;
                    break;
                }
            }
            if (deleteTask.second == 0) {
//                globalContext.gameObjectManager->deleteById(deleteTask.first);
//                shouldDelete.push_back(deleteTask.first);
//                frameInfo.gameObjects.erase(deleteTask.first);
                it = deleteTasks.erase(it);
            } else
                ++it;
        }
    }

    void TaskQueueSystem::roundTick() {
    }

    void TaskQueueSystem::sortTasks() {
        if (!tasksInQueue.empty()) {
            int cnt = -1;
//            std::cout << tasksInQueue.size() << std::endl;
            for (auto &i: tasksInQueue) {
//                if (cnt == 0 && !i.move) {
//                    PRINT(i.center);
//                    PRINT(points[cnt]);
//                }
//                std::cout << "#" << std::endl;
//                std::cout << i.move << std::endl;
//                PRINT(i.center);
//                PRINT(points[cnt]);
//                std::cout << "#" << std::endl;
                ++cnt;
                if (i.move)continue;
                globalContext.moveTaskManager->addMoveTask(i, points[cnt], FRAME_TIME/5);
            }
        }

        while (tasksInQueue.size() < TASK_NUM && !tasksWait.empty()) {
            auto now = std::move(tasksWait.front());
            tasksWait.pop();
            std::vector<glm::vec3> target = {points[TASK_NUM - 1], points[tasksInQueue.size()]};
//            glm::vec3 target = points[TASK_NUM - 1];
//            std::cout<<tasksInQueue.size()<<std::endl;
            tasksInQueue.emplace_back(now);
            globalContext.moveTaskManager->addMoveTask(tasksInQueue.back(), target);
        }
    }

    void TaskQueueSystem::addRunTask(Rectangle &task, glm::vec3 &point) {
        std::vector<glm::vec3> destinations;
        auto &center = task.center;
        destinations.emplace_back(MID(center[0], point[0]), center[1], TASK_LAYER);
        destinations.emplace_back(MID(center[0], point[0]), point[1], TASK_LAYER);
        destinations.emplace_back(point[0], point[1], TASK_LAYER);
        moveTasks.emplace_back(std::make_unique<TaskToMove>(task, destinations));
    }

    void TaskQueueSystem::addDeleteTask(Rectangle &task) {
        deleteTasks.emplace_back(task.id, FRAME_TIME);
    }

    void TaskQueueSystem::addTask() {
    }

    bool TaskQueueSystem::isSorted() {
        if (tasksInQueue.empty())return false;
//        PRINT(tasksInQueue.front().center);1111
        return STRICT_EQUAL(tasksInQueue.front().center, points[0]);
//        int cnt = 0;
//        for (auto &i: tasksInQueue) {
//            if (i.center != points[cnt])return false;
//            ++cnt;
//        }
//        return true;
    }

//    void taskList::run(Thread &thread) {
//    }

    void Task::run(glm::vec3 &point) {
        std::vector<glm::vec3> destinations;
        destinations.emplace_back(MID(center[0], point[0]), center[1], TASK_LAYER);
        destinations.emplace_back(MID(center[0], point[0]), point[1], TASK_LAYER);
        destinations.emplace_back(point[0], point[1], TASK_LAYER);
        globalContext.moveTaskManager->moveTasks.emplace_back(std::make_unique<TaskToMove>(*this, destinations));
    }
}

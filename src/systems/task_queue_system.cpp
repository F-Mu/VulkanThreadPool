#include "task_queue_system.hpp"
#include "../first_app.hpp"
#include "../global/global_context.hpp"

namespace crp {
    TaskQueueSystem::TaskQueueSystem(crp::CrpDevice &crpDevice,
                                     const std::shared_ptr<GameObjectManager> &manager) {
        auto TaskQueueRect = CrpGameObject::makeRectangle(crpDevice,
                                                          x, y, z, w, false);
        id = TaskQueueRect.getId();
        manager->gameObjects.emplace(TaskQueueRect.getId(), std::move(TaskQueueRect));
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
//        std::cout<<moveTasks.size()<<std::endl;
        for (auto it = moveTasks.begin(); it != moveTasks.end();) {
            auto moveTask = it->get();
//            std::cout<<moveTask->rectangle.points.size()<<' '<<moveTasks.size()<<std::endl;
            for (auto &kv: frameInfo.gameObjects) {
                if (kv.first == moveTask->rectangle.id) {
                    moveTask->tick();
                    kv.second.transform.translation = moveTask->rectangle.center;
                    break;
                }
            }
            if (moveTask->isFinished()) {
                it = moveTasks.erase(it);
            } else
                ++it;
        }
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
        sortTasks();
    }

    void TaskQueueSystem::sortTasks() {
        if (!tasksInQueue.empty() && tasksInQueue.front().center != points[0]) {
//            std::cout<<tasksInQueue.front().center[0]<<' '<<points[0][0]<<std::endl;
            glm::vec3 offset = points[0] - tasksInQueue.front().center;
            for (auto &i: tasksInQueue) {
                glm::vec3 target = i.center + offset;
                addMoveTask(i, target);
            }
        }
        while (tasksInQueue.size() < TASK_NUM && !tasksWait.empty()) {
            auto now = std::move(tasksWait.front());
            tasksWait.pop();
            std::vector<glm::vec3> targets = {points[TASK_NUM - 1], points[tasksInQueue.size()]};
            tasksInQueue.emplace_back(now);
            addMoveTask(tasksInQueue.back(), targets);
        }
    }

    void TaskQueueSystem::addMoveTask(Rectangle &task, glm::vec3 &point) {
        moveTasks.emplace_back(std::make_unique<TaskToMove>(task, point));
    }

    void TaskQueueSystem::addMoveTask(Rectangle &task, std::vector<glm::vec3> &point) {
        moveTasks.emplace_back(std::make_unique<TaskToMove>(task, point));
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

    void TaskQueueSystem::addTask(CrpDevice &crpDevice, const std::shared_ptr<GameObjectManager> &manager) {
        Rectangle task;
        task.center = taskInitPosition;
        task.points.resize(4);
        float l = -taskWidth / 2, r = taskWidth / 2,
                u = -taskHeight / 2, d = taskHeight / 2;
        task.points = {
                {l, u, TASK_LAYER},
                {r, u, TASK_LAYER},
                {l, d, TASK_LAYER},
                {r, d, TASK_LAYER},
        };
        auto TaskRect = CrpGameObject::makeRectangle(crpDevice, task.points, task.center, true, {0, .5f, .5f});
        task.id = TaskRect.getId();
        manager->gameObjects.emplace(TaskRect.getId(), std::move(TaskRect));
        tasksWait.push(task);
    }

}

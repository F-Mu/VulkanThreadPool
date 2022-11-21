#include "task_queue_system.hpp"
#include "../first_app.hpp"
namespace crp {
    TaskQueueSystem::TaskQueueSystem(crp::CrpDevice &crpDevice,
                                     const std::shared_ptr<GameObjectManager> &manager) {
        auto TaskQueueRect = CrpGameObject::makeRectangle(crpDevice,
                                                          x, y, z, w, false);
        id = TaskQueueRect.getId();
        manager->gameObjects.emplace(TaskQueueRect.getId(), std::move(TaskQueueRect));
        float mid = (left + right) / 2;
        points.resize(TASK_NUM);
        tasks.resize(TASK_NUM);
        float hCut = (down - up) / float(TASK_NUM);
        float yFirst = (up + hCut + up) / 2;
        float taskWidth = right - left - .15f;
        float taskHeight = (hCut - .1f);
        for (int i = 0; i < TASK_NUM; ++i) {
            points[i] = {mid, yFirst + hCut * i, TASK_LAYER};
            tasks[i].center = points[i];
            tasks[i].points.resize(4);
            float l = -taskWidth / 2, r = taskWidth / 2,
                    u = -taskHeight / 2, d = taskHeight / 2;
//            std::cout << l << ' ' << r << ' ' << u << ' ' << d << ' ' << taskUp << ' ' << taskHeight << ' ' << hCut
//                      << std::endl;
            tasks[i].points = {
                    {l, u, TASK_LAYER},
                    {r, u, TASK_LAYER},
                    {l, d, TASK_LAYER},
                    {r, d, TASK_LAYER},
            };
            auto TaskRect = CrpGameObject::makeRectangle(crpDevice,
                                                         tasks[i].points, points[i], true, {0, .5f, .5f});
            tasks[i].id = TaskRect.getId();
            manager->gameObjects.emplace(TaskRect.getId(), std::move(TaskRect));
        }
    }

    void TaskQueueSystem::tick(FrameInfo &frameInfo) {
        for (auto it = moveTasks.begin(); it != moveTasks.end();) {
            auto &moveTask = *it;
            for (auto &kv: frameInfo.gameObjects) {
                if (kv.first == moveTask.rectangle.id) {
                    moveTask.tick();
                    kv.second.transform.translation += moveTask.speed;
                    break;
                }
            }
            if (it->isFinished())
                it = moveTasks.erase(it);
            else
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

//                shouldDelete.push_back(deleteTask.first);
//                frameInfo.gameObjects.erase(deleteTask.first);
                it = deleteTasks.erase(it);
            } else
                ++it;
        }
    }


    void TaskQueueSystem::addMoveTask(Rectangle &task, glm::vec3 &point) {
        moveTasks.emplace_back(task, point);
    }

    void TaskQueueSystem::addDeleteTask(Rectangle &task) {
        deleteTasks.emplace_back(task.id, FRAME_TIME);
    }
}

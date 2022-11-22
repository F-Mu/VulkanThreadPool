#include "move_task_manager.hpp"
#include "../crp_frame_info.hpp"

namespace crp {

    void MoveTaskManager::addMoveTask(Rectangle &task, glm::vec3 &point, float time) {
        moveTasks.emplace_back(std::make_unique<TaskToMove>(task, point, time));
    }

    void MoveTaskManager::addMoveTask(Rectangle &task, std::vector<glm::vec3> &points, float time) {
        moveTasks.emplace_back(std::make_unique<TaskToMove>(task, points, time));
    }

    void MoveTaskManager::tick(FrameInfo &frameInfo) {
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
    }
}
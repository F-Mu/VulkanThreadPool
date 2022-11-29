#include "move_task_manager.hpp"
#include "render/crp_frame_info.hpp"

namespace crp {

    void MoveTaskManager::addMoveTask(Rectangle &task, glm::vec3 &point, float time) {
        if (task.move)return;
        moveTasks.emplace_back(std::make_unique<MoveTask>(task, point, time));
    }

    void MoveTaskManager::addMoveTask(Rectangle &task, std::vector<glm::vec3> &points, float time) {
        if (task.move)return;
        moveTasks.emplace_back(std::make_unique<MoveTask>(task, points, time));
    }

    void MoveTaskManager::tick(FrameInfo &frameInfo) {
        auto it = moveTasks.begin();
        for (int i = 0; i < moveTasks.size() && it != moveTasks.end(); ++i) {
            auto moveTask = it->get();
            if (!moveTask) {
                it = moveTasks.erase(it);
                continue;
            }
            moveTask->tick();
            frameInfo.gameObjects.find(moveTask->rectangle.id)
                    ->second.transform.translation = moveTask->rectangle.center;
            if (moveTask->isFinished()) {
                it = moveTasks.erase(it);
            } else
                ++it;
        }
    }
}
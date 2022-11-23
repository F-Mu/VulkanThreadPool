#pragma once

#include <list>
#include <memory>

#include "../core/rectangle.hpp"
namespace crp {
    class FrameInfo;
    class MoveTaskManager {
    public:
        std::list<std::unique_ptr<TaskToMove>> moveTasks;

        void addMoveTask(Rectangle &task, glm::vec3 &point,float time=FRAME_TIME);

        void addMoveTask(Rectangle &task, std::vector<glm::vec3> &point,float time=FRAME_TIME);

        void tick(FrameInfo &frameInfo);
    };
}
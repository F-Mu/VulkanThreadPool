#include "task.hpp"

namespace crp {
    glm::vec3 Task::initPosition = {};
    float Task::taskHeight = 0;
    float Task::taskWidth = 0;
    float Task::l = 0, Task::r = 0, Task::u = 0, Task::d = 0;
    std::vector<glm::vec3>Task::meshPoints{};

    void Task::run(glm::vec3 &point) {
        std::vector<glm::vec3> destinations;
        auto center = getCenter();
        destinations.emplace_back(MID(center[0], point[0]), center[1], TASK_LAYER);
        destinations.emplace_back(MID(center[0], point[0]), point[1], TASK_LAYER);
        destinations.emplace_back(point[0], point[1], TASK_LAYER);
        Move(destinations);
    }
}
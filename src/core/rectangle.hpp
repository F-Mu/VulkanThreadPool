#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "macro.hpp"
#include <iostream>

namespace crp {
    class Rectangle {
    public:
        using id_t = unsigned int;

        void MoveToPoint(glm::vec3 &direction, glm::vec3 &destination);

    public:
        std::vector<glm::vec3> points;
        glm::vec3 center;
        id_t id;
    };

    class TaskToMove {
    public:
        Rectangle rectangle;
//        glm::vec3 speed;
        glm::vec3 direction;
        std::vector<glm::vec3> destinations;
        std::vector<float> times;
//        float len;

        TaskToMove(Rectangle &rectangle, glm::vec3 &destination);

        TaskToMove(Rectangle &rectangle, std::vector<glm::vec3> &destinations);

        bool isFinished() const { return now == destinations.size(); };

        void tick();

    private:
        int now = 0;
        float speed;
    };
}
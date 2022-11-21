#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "macro.hpp"
#include <iostream>

namespace crp {
    class Rectangle {
    public:
        using id_t = unsigned int;

        void MoveToPoint(glm::vec3 &speed);

    public:
        std::vector<glm::vec3> points;
        glm::vec3 center;
        id_t id;
    };

    class TaskToMove {
    public:
        Rectangle rectangle;
        glm::vec3 destination;
        glm::vec3 speed;

        TaskToMove(Rectangle &rectangle, glm::vec3 &destination) : rectangle{rectangle},
                                                                   destination{destination} {
            auto direction = destination - rectangle.center;
            speed = direction / FRAME_TIME;
        }

        bool isFinished() const {
            auto i = rectangle.center - destination;
            return fabs(i[0]) < EPS && fabs(i[1]) < EPS;
        };

        void tick() {
            rectangle.MoveToPoint(speed);
        }
    };
}
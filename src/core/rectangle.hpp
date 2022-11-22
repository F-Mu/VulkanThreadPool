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

//        Rectangle(){};
//        Rectangle(Rectangle&&){}
        id_t id;
    public:
        std::vector<glm::vec3> points{};
        glm::vec3 center{};
        bool move{};
    };

    class TaskToMove {
    public:
        Rectangle &rectangle;
//        glm::vec3 speed;
        glm::vec3 direction;
        std::vector<glm::vec3> destinations;
        std::vector<float> times;
//        float len;

        TaskToMove(Rectangle &rectangle, glm::vec3 &destination, float time = FRAME_TIME);

        TaskToMove(Rectangle &rectangle, std::vector<glm::vec3> &destinations, float time = FRAME_TIME);

        bool isFinished() const {
            if (now == destinations.size()) {
                rectangle.move = false;
                return true;
            }
            return false;
        };

        void tick();

    private:
        int now = 0;
        float speed;
    };
}
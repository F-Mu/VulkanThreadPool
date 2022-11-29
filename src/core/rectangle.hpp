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

        id_t id;
    public:
        std::vector<glm::vec3> points{};
        glm::vec3 center{};
        bool move = false;
        bool movable = false;
    };

    class MoveTask {
    public:
        Rectangle &rectangle;
        glm::vec3 direction;
        std::vector<glm::vec3> destinations;
        std::vector<float> times;

        MoveTask(Rectangle &rectangle, glm::vec3 &destination, float time = FRAME_TIME);

        MoveTask(Rectangle &rectangle, std::vector<glm::vec3> &destinations, float time = FRAME_TIME);

        bool isFinished() {//会出现未完全生成的矩形，也可能把三个框莫名其妙移动了，补丁
            if (now == destinations.size() || rectangle.points.size() != 4 || !rectangle.movable) {
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
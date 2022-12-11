#pragma once

#include <glm/glm.hpp>
#include <thread>
#include "function/framework/rectangle.hpp"

namespace crp {
    class Thread : public Rectangle {
    public:
        Thread(std::vector<glm::vec3> points, glm::vec3 color = {1., 1., 1.}, bool fill = false,
               bool moveAble = false) :
                Rectangle(Rectangle::MakeRectangle(std::move(points), color, fill, moveAble)) {};
        std::thread th;
    };
}
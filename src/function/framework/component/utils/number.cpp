#include "number.hpp"
#include <iostream>

namespace crp {
    std::function<std::vector<Vertex>()> Number::get[10] = {
            []() {//get0
                std::vector<Vertex> vertices;
                addEdge(1);
                addEdge(2);
                addEdge(3);
                addEdge(5);
                addEdge(6);
                addEdge(7);
                return vertices;
            },
            []() {//get1
                std::vector<Vertex> vertices;
                //正常数字显示器的1没有底部衬托很奇怪，选择在中间
                addLine(vertices, getMidUp(), getMidMid());
                addLine(vertices, getMidMid(), getMidDown());
//                addEdge(3);
//                addEdge(6);
                return vertices;
            },
            []() {//get2
                std::vector<Vertex> vertices;
                addEdge(1);
                addEdge(3);
                addEdge(4);
                addEdge(5);
                addEdge(7);
                return vertices;
            },
            []() {//get3
                std::vector<Vertex> vertices;
                addEdge(1);
                addEdge(3);
                addEdge(4);
                addEdge(6);
                addEdge(7);
                return vertices;
            },
            []() {//get4
                std::vector<Vertex> vertices;
                addEdge(2);
                addEdge(3);
                addEdge(4);
                addEdge(6);
                return vertices;
            },
            []() {//get5
                std::vector<Vertex> vertices;
                addEdge(1);
                addEdge(2);
                addEdge(4);
                addEdge(6);
                addEdge(7);
                return vertices;
            },
            []() {//get6
                std::vector<Vertex> vertices;
                addEdge(1);
                addEdge(2);
                addEdge(4);
                addEdge(5);
                addEdge(6);
                addEdge(7);
                return vertices;
            },

            []() {//get7
                std::vector<Vertex> vertices;
                addEdge(1);
                addEdge(3);
                addEdge(6);
                return vertices;
            },
            []() {//get8
                std::vector<Vertex> vertices;
                addEdge(1);
                addEdge(2);
                addEdge(3);
                addEdge(4);
                addEdge(5);
                addEdge(6);
                addEdge(7);
                return vertices;
            },
            []() {//get9
                std::vector<Vertex> vertices;
                addEdge(1);
                addEdge(2);
                addEdge(3);
                addEdge(4);
                addEdge(6);
                addEdge(7);
                return vertices;
            }
    };

    float Number::downY{};
    float Number::upY{};
    float Number::leftX{};
    float Number::rightX{};
    glm::vec3 Number::color{};
    std::mutex Number::mut{};

    std::vector<Vertex> Number::getNumber(float width, float height, glm::vec3 _color, long long number) {
        std::lock_guard<std::mutex> lock(mut);
        color = _color;
        rightX = width / 2;
        leftX = -rightX;
        downY = height / 2;
        upY = -downY;
        return get[number]();
    }

    glm::vec3 Number::getLeftUp() {
        return {leftX, upY, NUMBER_LAYER};
    }

    glm::vec3 Number::getLeftMid() {
        return {leftX, 0, NUMBER_LAYER};
    }

    glm::vec3 Number::getLeftDown() {
        return {leftX, downY, NUMBER_LAYER};
    }

    glm::vec3 Number::getRightUp() {
        return {rightX, upY, NUMBER_LAYER};
    }

    glm::vec3 Number::getRightMid() {
        return {rightX, 0, NUMBER_LAYER};
    }

    glm::vec3 Number::getRightDown() {
        return {rightX, downY, NUMBER_LAYER};
    }

    glm::vec3 Number::getMidUp() {
        return {0, upY, NUMBER_LAYER};
    }

    glm::vec3 Number::getMidMid() {
        return {0, 0, NUMBER_LAYER};
    }

    glm::vec3 Number::getMidDown() {
        return {0, downY, NUMBER_LAYER};
    }

    void Number::addLine(std::vector<Vertex> &vertices, glm::vec3 &&x, glm::vec3 &&y) {
        auto tmp = std::move(Geometry::line(x, y, color));
        vertices.insert(vertices.end(), tmp.begin(), tmp.end());
    }

//    std::vector<Vertex> Number::get1() {
//        std::vector<Vertex> vertices;
//        addLine(vertices, getMidUp(), getMidMid());
//        addLine(vertices, getMidMid(), getMidDown());
//        return vertices;
//    }
//
//    std::vector<Vertex> Number::get2() {
//        std::vector<Vertex> vertices;
//        addEdge(1);
//        addEdge(3);
//        addEdge(4);
//        addEdge(5);
//        addEdge(7);
//        return vertices;
//    }
//
//    std::vector<Vertex> Number::get3() {
//        std::vector<Vertex> vertices;
//        addEdge(1);
//        addEdge(3);
//        addEdge(4);
//        addEdge(6);
//        addEdge(7);
//        return vertices;
//    }
//
//    std::vector<Vertex> Number::get4() {
//        std::vector<Vertex> vertices;
//        addEdge(2);
//        addEdge(3);
//        addEdge(4);
//        addEdge(6);
//        return vertices;
//    }
//
//    std::vector<Vertex> Number::get5() {
//        std::vector<Vertex> vertices;
//        addEdge(1);
//        addEdge(2);
//        addEdge(4);
//        addEdge(6);
//        addEdge(7);
//        return vertices;
//    }
//
//    std::vector<Vertex> Number::get6() {
//        std::vector<Vertex> vertices;
//        addEdge(1);
//        addEdge(2);
//        addEdge(4);
//        addEdge(5);
//        addEdge(6);
//        addEdge(7);
//        return vertices;
//    }
//
//    std::vector<Vertex> Number::get7() {
//        std::vector<Vertex> vertices;
//        addEdge(1);
//        addEdge(3);
//        addEdge(6);
//        return vertices;
//    }
//
//    std::vector<Vertex> Number::get8() {
//        std::vector<Vertex> vertices;
//        addEdge(1);
//        addEdge(2);
//        addEdge(3);
//        addEdge(4);
//        addEdge(5);
//        addEdge(6);
//        addEdge(7);
//        return vertices;
//    }
//
//    std::vector<Vertex> Number::get9() {
//        std::vector<Vertex> vertices;
//        addEdge(1);
//        addEdge(2);
//        addEdge(3);
//        addEdge(4);
//        addEdge(6);
//        addEdge(7);
//        return vertices;
//    }

    void Number::addEdge1(std::vector<Vertex> &vertices) {
        addLine(vertices, getLeftUp(), getRightUp());
    }

    void Number::addEdge2(std::vector<Vertex> &vertices) {
        addLine(vertices, getLeftUp(), getLeftMid());
    }

    void Number::addEdge3(std::vector<Vertex> &vertices) {
        addLine(vertices, getRightUp(), getRightMid());
    }

    void Number::addEdge4(std::vector<Vertex> &vertices) {
        addLine(vertices, getLeftMid(), getRightMid());
    }

    void Number::addEdge5(std::vector<Vertex> &vertices) {
        addLine(vertices, getLeftMid(), getLeftDown());
    }

    void Number::addEdge6(std::vector<Vertex> &vertices) {
        addLine(vertices, getRightMid(), getRightDown());
    }

    void Number::addEdge7(std::vector<Vertex> &vertices) {
        addLine(vertices, getLeftDown(), getRightDown());
    }
}
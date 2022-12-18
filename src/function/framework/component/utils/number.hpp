#pragma once

#include "geometry.hpp"
#include "core/macro.hpp"
#include <mutex>

namespace crp {
    class Number {
    public:
        static std::vector<Vertex> getNumber(float width, float height, glm::vec3 _color, long long number);
//        static std::vector<Vertex> get1();
//
//        static std::vector<Vertex> get2();
//
//        static std::vector<Vertex> get3();
//
//        static std::vector<Vertex> get4();
//
//        static std::vector<Vertex> get5();
//
//        static std::vector<Vertex> get6();
//
//        static std::vector<Vertex> get7();
//
//        static std::vector<Vertex> get8();
//
//        static std::vector<Vertex> get9();

    private:
        static float downY, upY, leftX, rightX;
        static glm::vec3 color;
//        static constexpr float offset = 0.0025;
        static std::mutex mut;
        static std::function<std::vector<Vertex>()> get[10];

        static glm::vec3 getLeftUp();

        static glm::vec3 getLeftMid();

        static glm::vec3 getLeftDown();

        static glm::vec3 getRightUp();

        static glm::vec3 getRightMid();

        static glm::vec3 getRightDown();

        static glm::vec3 getMidUp();

        static glm::vec3 getMidMid();

        static glm::vec3 getMidDown();

        static void addLine(std::vector<Vertex> &vertices, glm::vec3 &&x, glm::vec3 &&y);

        static void addEdge1(std::vector<Vertex> &vertices);

        static void addEdge2(std::vector<Vertex> &vertices);

        static void addEdge3(std::vector<Vertex> &vertices);

        static void addEdge4(std::vector<Vertex> &vertices);

        static void addEdge5(std::vector<Vertex> &vertices);

        static void addEdge6(std::vector<Vertex> &vertices);

        static void addEdge7(std::vector<Vertex> &vertices);
//1～7号边为从上到下，从左到右的日字形的边
//#define get(number) get##number()
#define addEdge(number) addEdge##number(vertices)
    };
}
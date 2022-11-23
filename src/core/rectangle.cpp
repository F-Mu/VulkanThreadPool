
#include "rectangle.hpp"
#include <iostream>

namespace crp {

    void Rectangle::MoveToPoint(glm::vec3 &direction, glm::vec3 &destination) {
        direction[2] = 0;
        int flag, pos, flag2;
        if (fabs(center[0] - destination[0]) < EPS)flag = center[1] - destination[1] > 0 ? 1 : -1, pos = 1;
        else flag = center[0] - destination[0] > 0 ? 1 : -1, pos = 0;
        if (EQUAL(center, destination)) {
            center[0] = destination[0];
            center[1] = destination[1];
            return;
        }
        for (auto &i: points) {
            i += direction;
        }
        center += direction;
        flag2 = center[pos] - destination[pos] > 0 ? 1 : -1;
        if (flag != flag2) {
            center = destination;
        }
    }

    void TaskToMove::tick() {
        if (rectangle.points.size() != 4)return;//补丁
//        std::cout<<now<<' '<<destinations.size()<<std::endl;
        if (isFinished())return;
        rectangle.MoveToPoint(direction, destinations[now]);
        if (rectangle.center == destinations[now]) {
            ++now;
            if (isFinished())return;
            auto dir = destinations[now] - destinations[now - 1];
//            direction = dir / (times[now] * FRAME_TIME);
            direction = NORMALIZE(dir) * speed;
        }
    }

    TaskToMove::TaskToMove(Rectangle &rectangle, std::vector<glm::vec3> &destinations, float time) : rectangle{
            rectangle},
                                                                                                     destinations{
                                                                                                             destinations} {
        auto dir = destinations[0] - rectangle.center;
        float len = LEN(dir);
        for (int i = 1; i < destinations.size(); ++i) {
            auto d = destinations[i] - destinations[i - 1];
            len += LEN(d);
        }
        times.resize(destinations.size());
        times[0] = LEN(dir) / len;
        for (int i = 1; i < destinations.size(); ++i) {
            auto d = destinations[i] - destinations[i - 1];
            times[i] = LEN(d) / len;
        }
        speed = len / time;
        direction = NORMALIZE(dir) * speed;
        rectangle.move = true;
//        direction = dir / (times[0] * FRAME_TIME);
    }

    TaskToMove::TaskToMove(Rectangle &rectangle, glm::vec3 &destination, float time) : rectangle{rectangle},
                                                                                       destinations{destination} {
        auto dir = destination - rectangle.center;
//        float len = LEN(dir);
//        speed = len / FRAME_TIME;
        times.emplace_back(time);
        direction = dir / time;
        rectangle.move = true;
//        std::cout << direction[0] << ' ' << direction[1] << '#' << destination[0] << ' ' << destination[1] << std::endl;
    }
}
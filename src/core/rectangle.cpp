
#include "rectangle.hpp"
#include <iostream>

namespace crp {

    void Rectangle::MoveToPoint(glm::vec3 &speed) {
        for (auto &i: points) {
            i += speed;
        }
//        for(auto&i:points)
//            std::cout<<i[0]<<' '<<i[1]<<' '<<i[2]<<' '<<speed[0]<<' '<<speed[1]<<' '<<speed[2]<<std::endl;
        center += speed;
//        std::cout<<center[0]<<' '<<center[1]<<std::endl;
    }

}
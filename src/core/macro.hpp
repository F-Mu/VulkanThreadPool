#pragma once

#include <glm/glm.hpp>

namespace crp {
    static constexpr float QUEUE_LAYER = 0.1;
    static constexpr float TASK_LAYER = 0.2;
    static constexpr float THREAD_LAYER = 0.3;
    const int TASK_NUM = 8;
    const int THREAD_NUM = 5;
    const glm::vec3 SCALE{1.f, 1.f, 1.f};
    const float FRAME_TIME = 60.f;
    const float EPS = 1e-2;
    const float STRICT_EPS = 1e-7;
    const glm::vec3 TASK_COLOR = {0, .5f, .5f};
    const glm::vec3 THREAD_COLOR = {0, 0, .5f};

    inline bool EQUAL(const glm::vec3 &vec3a, const glm::vec3 &vec3b) {
        return fabs(vec3a[0] - vec3b[0]) < EPS && fabs(vec3a[1] - vec3b[1]) < EPS;
    }

    inline bool STRICT_EQUAL(const glm::vec3 &vec3a, const glm::vec3 &vec3b) {
        return fabs(vec3a[0] - vec3b[0]) < STRICT_EPS && fabs(vec3a[1] - vec3b[1]) < STRICT_EPS;
    }

    inline float MID(const float &x, const float &y) {
        return (x + y) / 2;
    }

#define PRINT(vec) std::cout<<'('<<(vec)[0]<<','<<(vec)[1]<<','<<(vec)[2]<<')'<<std::endl
#define PRINT4(vec) std::cout<<'('<<(vec)[0]<<','<<(vec)[1]<<','<<(vec)[2]<<','<<(vec)[3]<<')'<<std::endl
}
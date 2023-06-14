#pragma once

#include <glm/glm.hpp>
/**
 #define -> const + inline《Effective C++》条款02:尽量以const,enum,inline替换#define
 inline -> inline + noexcept 《Effective Modern C++》条款14:只要函数不会发射异常，就为其加上noexcept声明
 const -> constexpr 《Effective Modern C++》条款15:只要有可能使用constexpr，就使用它
*/
namespace crp {
    static constexpr float QUEUE_LAYER = 0.1;
    static constexpr float PARTICLE_LAYER = 0.2;
    static constexpr float NUMBER_LAYER = 0.2;
    static constexpr float TASK_LAYER = 0.3;
    static constexpr float THREAD_LAYER = 0.4;
    constexpr int TASK_NUM = 8;
    constexpr int THREAD_NUM = 5;
    constexpr glm::vec3 SCALE{1.f, 1.f, 1.f};
    constexpr float FRAME_TIME = 60.f;
    constexpr float EPS = 1e-7;
    constexpr float STRICT_EPS = 1e-7;
    constexpr glm::vec3 TASK_COLOR = {0, .5f, .5f};
    constexpr glm::vec3 THREAD_COLOR = {0, 0, .5f};

    inline bool EQUAL(const glm::vec3 &vec3a, const glm::vec3 &vec3b) noexcept {
        return fabs(vec3a[0] - vec3b[0]) < EPS && fabs(vec3a[1] - vec3b[1]) < EPS;
    }

    inline bool FIX_EQUAL(glm::vec3 &vec3a, const glm::vec3 &vec3b) noexcept {
        if (fabs(vec3a[0] - vec3b[0]) < EPS && fabs(vec3a[1] - vec3b[1]) < EPS) {
            vec3a[0] = vec3b[0];
            vec3a[1] = vec3b[1];
            return true;
        }
        return false;
    }

    inline bool STRICT_EQUAL(const glm::vec3 &vec3a, const glm::vec3 &vec3b) noexcept {
        return fabs(vec3a[0] - vec3b[0]) < STRICT_EPS && fabs(vec3a[1] - vec3b[1]) < STRICT_EPS;
    }

    inline float MID(const float &x, const float &y) noexcept {
        return (x + y) / 2;
    }

#define PRINT(vec) std::cout<<'('<<(vec)[0]<<','<<(vec)[1]<<','<<(vec)[2]<<')'<<std::endl
#define PRINT4(vec) std::cout<<'('<<(vec)[0]<<','<<(vec)[1]<<','<<(vec)[2]<<','<<(vec)[3]<<')'<<std::endl
#define LOG(message) std::cout<<message<<std::endl;
}
#pragma once
namespace crp {
#define QUEUE_LAYER (-3)
#define TASK_LAYER (-1)
#define THREAD_LAYER (0)
//#define FRAME_TIME 60
#define FRAME_TIME (120.f)
#define EPS (1e-2)
#define TASK_NUM 8
#define THREAD_NUM 5
#define SCALE glm::vec3{1.f, 1.f, 1.f}
#define LEN(vec3) sqrt(vec3[0]*vec3[0]+vec3[1]*vec3[1])
#define EQUAL(vec3a, vec3b) (fabs(vec3a[0]-vec3b[0])<EPS&&fabs(vec3a[1]-vec3b[1])<EPS)
#define NORMALIZE(vec) glm::vec3{vec[0]/LEN(vec),vec[1]/LEN(vec),0.f}
#define PRINT(vec) std::cout<<'('<<vec[0]<<','<<vec[1]<<','<<vec[2]<<')'<<std::endl;
#define MID(x, y) ((x+y)/2)
}
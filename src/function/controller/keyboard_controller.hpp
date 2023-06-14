#pragma once

#include "function/framework/game_object.hpp"
#include "render/window_system.hpp"

namespace crp {
    class KeyboardController {
    public:
        struct KeyMappings {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_E;
            int moveDown = GLFW_KEY_Q;
            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;
            int add1 = GLFW_KEY_1;
            int add2 = GLFW_KEY_2;
            int add3 = GLFW_KEY_3;
            int add4 = GLFW_KEY_4;
            int add5 = GLFW_KEY_5;
            int add6 = GLFW_KEY_6;
            int add7 = GLFW_KEY_7;
            int add8 = GLFW_KEY_8;
            int add9 = GLFW_KEY_9;
            int debug = GLFW_KEY_M;
        };

        void moveInPlaneXZ(GLFWwindow *window, float dt, GameObject &gameObject);

        void addTask(GLFWwindow *window);

        KeyMappings keys{};
        float moveSpeed{3.f};
        float lookSpeed{1.5f};

        bool add[10]{};
    };
}
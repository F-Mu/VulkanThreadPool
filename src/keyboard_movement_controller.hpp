#pragma once

#include "crp_game_obejct.hpp"
#include "crp_window.hpp"

namespace crp {
    class KeyboardMovementController {
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
            int debug = GLFW_KEY_M;
        };

        void moveInPlaneXZ(GLFWwindow *window, float dt, CrpGameObject &gameObject);

        void addTask(GLFWwindow *window);

        KeyMappings keys{};
        float moveSpeed{3.f};
        float lookSpeed{1.5f};

        bool add[6]{};
    };
}
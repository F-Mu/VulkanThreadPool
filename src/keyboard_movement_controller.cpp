#include "keyboard_movement_controller.hpp"
#include "systems/task_queue_system.hpp"
#include "systems/runtime_system.hpp"
#include "global/global_context.hpp"

namespace crp {
    void crp::KeyboardMovementController::moveInPlaneXZ(
            GLFWwindow *window, float dt, crp::CrpGameObject &gameObject) {
        glm::vec3 rotate(0);
        if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS)rotate.y += 1.f;
        if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS)rotate.y -= 1.f;
        if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS)rotate.x += 1.f;
        if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS)rotate.x -= 1.f;
        if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
            gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
        }

        gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
        gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

        float yaw = gameObject.transform.rotation.y;
        const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
        const glm::vec3 rightDir(forwardDir.z, 0.f, -forwardDir.x);
        const glm::vec3 upDir{0.f, -1.f, 0.f};

        glm::vec3 moveDir{0.f};
        if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS)moveDir += forwardDir;
        if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS)moveDir -= forwardDir;
        if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS)moveDir += rightDir;
        if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS)moveDir -= rightDir;
        if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS)moveDir += upDir;
        if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS)moveDir -= upDir;

        if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
            gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
        }
    }

    void KeyboardMovementController::addTask(GLFWwindow *window) {
        int time = 0;
        if (glfwGetKey(window, keys.add1) == GLFW_PRESS)add[0] = true;
        if (glfwGetKey(window, keys.add2) == GLFW_PRESS)add[1] = true;
        if (glfwGetKey(window, keys.add3) == GLFW_PRESS)add[2] = true;
        if (glfwGetKey(window, keys.add4) == GLFW_PRESS)add[3] = true;
        if (glfwGetKey(window, keys.add5) == GLFW_PRESS)add[4] = true;

        if (add[0] && glfwGetKey(window, keys.add1) == GLFW_RELEASE)time = 1,add[0]=false;
        if (add[1] && glfwGetKey(window, keys.add2) == GLFW_RELEASE)time = 2,add[1]=false;
        if (add[2] && glfwGetKey(window, keys.add3) == GLFW_RELEASE)time = 3,add[2]=false;
        if (add[3] && glfwGetKey(window, keys.add4) == GLFW_RELEASE)time = 4,add[3]=false;
        if (add[4] && glfwGetKey(window, keys.add5) == GLFW_RELEASE)time = 5,add[4]=false;

        if (time == 0)return;
        globalContext.runTimeSystem->taskQueueSystem->add(
                [time] { std::this_thread::sleep_for(std::chrono::seconds(time)); });
    }
}
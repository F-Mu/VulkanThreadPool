#include "window_system.hpp"

#include <stdexcept>

namespace crp {
    WindowSystem::~WindowSystem() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void WindowSystem::initialize(WindowCreateInfo create_info) {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        width=create_info.width;
        height=create_info.height;
        window = glfwCreateWindow(create_info.width, create_info.height, create_info.title, nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    void WindowSystem::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface");
        }
    }

    void WindowSystem::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto crpWindow = reinterpret_cast<WindowSystem *>(glfwGetWindowUserPointer(window));
        crpWindow->framebufferResized = true;
        crpWindow->width = width;
        crpWindow->height = height;
    }

}
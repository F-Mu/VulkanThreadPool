#pragma once
#define GLFW_INCLUDE_VULKAN

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <string>

namespace crp {
    class WindowSystem {
    public:
        WindowSystem();

        ~WindowSystem();

        WindowSystem(const WindowSystem &) = delete;

        WindowSystem &operator=(const WindowSystem &) = delete;

        bool shouldClose() { return glfwWindowShouldClose(window); }

        VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

        bool wasWindowResized() { return framebufferResized; }

        void resetWindowResizedFlag() { framebufferResized = false; }

        GLFWwindow *getGLFWwindow() const { return window; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    private:
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

        void init();

        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        int width;
        int height;
        bool framebufferResized = false;

        std::string windowName{"ThreadPool"};


        GLFWwindow *window;
    };
}// namespace crp
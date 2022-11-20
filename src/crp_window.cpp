#include "crp_window.hpp"

#include <stdexcept>
#include <utility>

namespace crp {
    CrpWindow::CrpWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{std::move(name)} {
        initWindow();
    }

    CrpWindow::~CrpWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void CrpWindow::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window,this);
        glfwSetFramebufferSizeCallback(window,framebufferResizeCallback);
    }

    void CrpWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface");
        }
    }

    void CrpWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto crpWindow=reinterpret_cast<CrpWindow*>(glfwGetWindowUserPointer(window));
        crpWindow->framebufferResized=true;
        crpWindow->width=width;
        crpWindow->height=height;
    }

}
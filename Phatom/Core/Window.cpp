#include "Window.h"

namespace phatom {
Window::Window(int width, int height) :
mWidth(width),
mHeight(height){
}

Window::~Window() {
    
}

void Window::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    mWindow = glfwCreateWindow(mWidth, mHeight, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(mWindow, this);
    glfwSetFramebufferSizeCallback(mWindow, framebufferResizeCallback);
}

void Window::destoryWindow() {
    glfwDestroyWindow(mWindow);
}

VkSurfaceKHR Window::createSurface(VkInstance instance) {
    if(instance == VK_NULL_HANDLE || !mWindow) {
        return VK_NULL_HANDLE;
    }
    
    VkResult errCode = glfwCreateWindowSurface(instance, mWindow, nullptr, &mSurface);
    if(errCode != VK_SUCCESS) {
        return VK_NULL_HANDLE;
    }
    return mSurface;
}

int Window::getWidth() {
    return mWidth;
}

int Window::getHeight() {
    return mHeight;
}

GLFWwindow* Window::getHandle() {
    return mWindow;
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(mWindow);
}

void Window::processEvents() {
    glfwPollEvents();
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto windowPtr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    windowPtr->mFramebufferResized = true;
}
} // namespace phatom

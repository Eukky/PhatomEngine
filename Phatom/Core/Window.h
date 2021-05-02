#ifndef PHATOM_WINDOW_H
#define PHATOM_WINDOW_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

//#include "Graphics/VulkanInstance.h"

namespace phatom {
class Window {
public:
    Window(int width, int height);
    ~Window();
    
    void initWindow();
    void destoryWindow();
    VkSurfaceKHR createSurface(VkInstance instance);
    int getWidth();
    int getHeight();
    GLFWwindow* getWindow();
    bool shouldClose();
    void processEvents();
    
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

//    static void WindowSizeCallback(GLFWwindow* window, int width, int height);
//    static void WindowCloseCallback(GLFWwindow* window);
//    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
//    static void CharCallback(GLFWwindow* window, unsigned int keycode);
//    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
//    static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
//    static void CursorCallback(GLFWwindow* window, double xPos, double yPos);
    
private:
    GLFWwindow* mWindow;
    VkSurfaceKHR mSurface;
    int mWidth;
    int mHeight;
    bool mFramebufferResized;
};
} // namespace phatom

#endif //PHATOM_WINDOW_H

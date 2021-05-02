#ifndef PHATOM_VULKANINSTANCE_H
#define PHATOM_VULKANINSTANCE_H

#include <Vulkan/Vulkan.h>
#include <vector>
#include "VulkanPhysicalDevice.h"
namespace phatom {
class VulkanInstance {
public:
    VulkanInstance(const char* applicationName, std::vector<const char*> validationLayers,  std::vector<const char *> extensions);
    ~VulkanInstance();

    VkInstance getHandle();
    VkResult createInstance();
    void setupDebugMessenger();
    void quaryGPUs();
    VulkanPhysicalDevice getSuitableGPU();
    
private:
    bool checkValidationLayerSupport(std::vector<const char*> validationLayers);
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
private:
    VkInstance mHandle;
    std::vector<VulkanPhysicalDevice*> mGPUs;
    const char* mApplicationName;
    std::vector<const char*> mValidationLayers;
    std::vector<const char*> mExtensions;
    VkDebugUtilsMessengerEXT mDebugMessenger;
    bool mIsDebug;
};

} // namespace phatom

#endif //PHATOM_VULKANINSTANCE_H

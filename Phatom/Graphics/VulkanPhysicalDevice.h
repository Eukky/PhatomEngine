#ifndef PHATOM_VULKANPHYSICALDEVICE_H
#define PHATOM_VULKANPHYSICALDEVICE_H

#include <vector>
#include <Vulkan/Vulkan.h>

namespace phatom {
class VulkanPhysicalDevice {
public:
    VulkanPhysicalDevice(VkPhysicalDevice physicalDevice);
    ~VulkanPhysicalDevice();
    
    VkPhysicalDevice getHandle();
    VkPhysicalDeviceProperties getProperties();
    std::vector<VkQueueFamilyProperties> getQueueFamilyProperties();
    VkPhysicalDeviceFeatures getRequestedFeatures();
    VkBool32 isPresentSupported(VkSurfaceKHR surface, uint32_t queueFamilyIndex);
private:
    VkPhysicalDevice mHandle;
    VkPhysicalDeviceFeatures mFeatures;
    VkPhysicalDeviceProperties mProperties;
    VkPhysicalDeviceMemoryProperties mMemoryProperties;
    std::vector<VkQueueFamilyProperties> mQueueFamilyProperties;
    VkPhysicalDeviceFeatures mRequestedFeatures{};
    
};
} // namespace phatom


#endif //PHATOM_VULKANPHYSICALDEVICE_H

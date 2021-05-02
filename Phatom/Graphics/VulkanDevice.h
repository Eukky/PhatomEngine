#ifndef PHATOM_VULKANDEVICE_H
#define PHATOM_VULKANDEVICE_H

#include <Vulkan/Vulkan.h>
#include <optional>
#include <unordered_map>
#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"
namespace phatom {
class VulkanDevice {
public:
    VulkanDevice(VulkanPhysicalDevice &gpu, VkSurfaceKHR surface, std::unordered_map<const char*, bool> requestedExtensions, std::vector<const char*> validationLayers);
    ~VulkanDevice();
    
    VkDevice getHandle();
    std::vector<VkQueue> getQueues();

private:
    bool isExtensionSupported(const std::string& requestedExtension);
    
private:
    VkDevice mHandle;
    VulkanPhysicalDevice &mGPU;
    VkSurfaceKHR mSurface;
    std::vector<VkQueue> mQueues;
    std::vector<VkExtensionProperties> mDeviceExtensions;
    std::vector<const char*> mEnabledExtensions;

};
} // namespace phatom


#endif //PHATOM_VULKANDEVICE_H

#ifndef PHATOM_VULKANDEVICE_H
#define PHATOM_VULKANDEVICE_H

#include <Vulkan/Vulkan.h>
#include <optional>
#include <unordered_map>
#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanQueue.h"
namespace phatom {
class VulkanDevice {
public:
    VulkanDevice(VulkanPhysicalDevice &gpu, VkSurfaceKHR surface, std::unordered_map<const char*, bool> requestedExtensions, std::vector<const char*> validationLayers);
    ~VulkanDevice();
    
    VkDevice getHandle();
    std::vector<std::vector<VulkanQueue>> getQueues();
    VulkanQueue& getQueue(uint32_t queuefamilyIndex, uint32_t queueIndex);
    VulkanQueue& getQueueByFlags(VkQueueFlags requiredQueueFlags, uint32_t queueIndex);
    VulkanQueue& getSuitableGraphicsQueue();

private:
    bool isExtensionSupported(const std::string& requestedExtension);
    
private:
    VkDevice mHandle;
    VulkanPhysicalDevice &mGPU;
    VkSurfaceKHR mSurface;
//    std::vector<VkQueue> mQueues;
    std::vector<std::vector<VulkanQueue>> mQueues;
    std::vector<VkExtensionProperties> mDeviceExtensions;
    std::vector<const char*> mEnabledExtensions;

};
} // namespace phatom


#endif //PHATOM_VULKANDEVICE_H

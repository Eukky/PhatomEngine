#include "VulkanPhysicalDevice.h"
#include <vector>
namespace phatom {

VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice physicalDevice) :
mHandle(physicalDevice){
    vkGetPhysicalDeviceFeatures(mHandle, &mFeatures);
    vkGetPhysicalDeviceProperties(mHandle, &mProperties);
    vkGetPhysicalDeviceMemoryProperties(mHandle, &mMemoryProperties);
    
//    LOGI("Found GPU: {}", properties.deviceName);
    
    uint32_t queueFamilyPropertiesCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(mHandle, &queueFamilyPropertiesCount, nullptr);
    mQueueFamilyProperties = std::vector<VkQueueFamilyProperties>(queueFamilyPropertiesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(mHandle, &queueFamilyPropertiesCount, mQueueFamilyProperties.data());
    
}

VulkanPhysicalDevice::~VulkanPhysicalDevice() {
    
}

VkPhysicalDevice VulkanPhysicalDevice::getHandle() {
    return mHandle;
}

VkPhysicalDeviceProperties VulkanPhysicalDevice::getProperties() {
    return mProperties;
}

std::vector<VkQueueFamilyProperties> VulkanPhysicalDevice::getQueueFamilyProperties() {
    return mQueueFamilyProperties;
}

VkPhysicalDeviceFeatures VulkanPhysicalDevice::getRequestedFeatures() {
    return mRequestedFeatures;
}

VkBool32 VulkanPhysicalDevice::isPresentSupported(VkSurfaceKHR surface, uint32_t queueFamilyIndex) {
    VkBool32 presentSupported{VK_FALSE};
    
    if(surface != VK_NULL_HANDLE) {
        vkGetPhysicalDeviceSurfaceSupportKHR(mHandle, queueFamilyIndex, surface, &presentSupported);
    }
    return presentSupported;
}
} // namespace phatom

#include "VulkanDevice.h"

#include <string>
namespace phatom {
VulkanDevice::VulkanDevice(VulkanPhysicalDevice &gpu, VkSurfaceKHR surface, std::unordered_map<const char*, bool> requestedExtensions, std::vector<const char*> validationLayers) :
mGPU(gpu),
mSurface(surface){
    uint32_t queueFamilyPropertiesCount = mGPU.getQueueFamilyProperties().size();
    
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos(queueFamilyPropertiesCount, {VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO});
    
    std::vector<std::vector<float>> queuePriorities(queueFamilyPropertiesCount);
    
    for(int32_t queueFamilyIndex = 0; queueFamilyIndex < queueFamilyPropertiesCount; ++queueFamilyIndex) {
        
        VkQueueFamilyProperties queueFamilyProperty = mGPU.getQueueFamilyProperties()[queueFamilyIndex];
        queuePriorities[queueFamilyIndex].resize(queueFamilyProperty.queueCount, 1.0f);
        
        VkDeviceQueueCreateInfo &queueCreateInfo = queueCreateInfos[queueFamilyIndex];
        
        queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
        queueCreateInfo.queueCount = queueFamilyProperty.queueCount;
        queueCreateInfo.pQueuePriorities = queuePriorities[queueFamilyIndex].data();
    }

    
    uint32_t deviceExtensionCount;
    vkEnumerateDeviceExtensionProperties(mGPU.getHandle(), nullptr, &deviceExtensionCount, nullptr);
    mDeviceExtensions = std::vector<VkExtensionProperties>(deviceExtensionCount);
    vkEnumerateDeviceExtensionProperties(mGPU.getHandle(), nullptr, &deviceExtensionCount, mDeviceExtensions.data());
    
    bool canGetMemoryRequirements = isExtensionSupported("VK_KHR_get_memory_requirements2");
    bool hasDedicatedAllocation = isExtensionSupported("VK_KHR_dedicated_allocation");
    
    if(canGetMemoryRequirements && hasDedicatedAllocation) {
        mEnabledExtensions.push_back("VK_KHR_get_memory_requirements2");
        mEnabledExtensions.push_back("VK_KHR_dedicated_allocation");
    }
    
//    if(isExtensionSupported("VK_KHR_performance_query") && isExtensionSupported("VK_EXT_host_query_reset")) {
//        auto perfCounterFeatures = mGPU.request_extension_features<VkPhysicalDevicePerformanceQueryFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_FEATURES_KHR);
//        auto hostQueryReset_features = mGPU.request_extension_features<VkPhysicalDeviceHostQueryResetFeatures>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES);
//
//        if (perfCounterFeatures.performanceCounterQueryPools && hostQueryReset_features.hostQueryReset)
//        {
//            mEnabledExtensions.push_back("VK_KHR_performance_query");
//            mEnabledExtensions.push_back("VK_EXT_host_query_reset");
//        }
//    }
    std::vector<const char *> unsupportedExtensions{};
    for(auto& extension : requestedExtensions) {
        if(isExtensionSupported(extension.first)) {
            mEnabledExtensions.push_back(extension.first);
        } else {
            unsupportedExtensions.push_back(extension.first);
        }
    }
    
    VkDeviceCreateInfo createInfo;
    
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//    createInfo.pNext = mGPU.getExtensionFeatureChain();
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = queueCreateInfos.size();
    createInfo.enabledExtensionCount = mEnabledExtensions.size();
    createInfo.ppEnabledExtensionNames = mEnabledExtensions.data();
    
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    //debug
    VkPhysicalDeviceFeatures requestedFeature = mGPU.getRequestedFeatures();
    createInfo.pEnabledFeatures = &requestedFeature;
    
    VkResult result = vkCreateDevice(mGPU.getHandle(), &createInfo, nullptr, &mHandle);
    
    if(result != VK_SUCCESS) {
        throw std::runtime_error("Cannot create device!");
    }
    
    mQueues.resize(queueFamilyPropertiesCount);
    for(uint32_t queueFamilyIndex = 0; queueFamilyIndex < queueFamilyPropertiesCount; ++queueFamilyIndex) {
        VkQueueFamilyProperties queueFamilyProperty = mGPU.getQueueFamilyProperties()[queueFamilyIndex];
        VkBool32 presentSupported = mGPU.isPresentSupported(mSurface, queueFamilyIndex);
        if(mSurface != VK_NULL_HANDLE) {
            vkGetPhysicalDeviceSurfaceSupportKHR(mGPU.getHandle(), queueFamilyIndex, mSurface, &presentSupported);
        }
        for(uint32_t queueIndex = 0; queueIndex < queueFamilyProperty.queueCount; ++queueIndex) {
            VkQueue queue{VK_NULL_HANDLE};
            vkGetDeviceQueue(mHandle, queueFamilyIndex, queueIndex, &queue);
            mQueues[queueFamilyIndex] = queue;
        }
    }
    
}

VulkanDevice::~VulkanDevice() {
    
}

VkDevice VulkanDevice::getHandle() {
    return mHandle;
}

std::vector<VkQueue> VulkanDevice::getQueues() {
    return mQueues;
}

bool VulkanDevice::isExtensionSupported(const std::string& requestedExtension) {
    return std::find_if(mDeviceExtensions.begin(), mDeviceExtensions.end(),
                        [requestedExtension](auto &deviceExtension) {
                            return std::strcmp(deviceExtension.extensionName, requestedExtension.c_str()) == 0;
                        }) != mDeviceExtensions.end();
}


} // namespace phatom

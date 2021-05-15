#include "VulkanDevice.h"
#include "Common/Log.h"
#include <string>
#include "VulkanConfig.h"
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

    if(kEnableDebug) {
        VkPhysicalDeviceFeatures requestedFeature = mGPU.getRequestedFeatures();
        createInfo.pEnabledFeatures = &requestedFeature;
    }
    
    VkResult result = vkCreateDevice(mGPU.getHandle(), &createInfo, nullptr, &mHandle);
    
    if(result != VK_SUCCESS) {
        throw std::runtime_error("Cannot create device!");
    }
    
    mQueues.resize(queueFamilyPropertiesCount);
    for(uint32_t queueFamilyIndex = 0; queueFamilyIndex < queueFamilyPropertiesCount; ++queueFamilyIndex) {
        VkQueueFamilyProperties queueFamilyProperty = mGPU.getQueueFamilyProperties()[queueFamilyIndex];
        VkBool32 presentSupported = mGPU.isPresentSupported(mSurface, queueFamilyIndex);
 
        for(uint32_t queueIndex = 0; queueIndex < queueFamilyProperty.queueCount; ++queueIndex) {
            VkQueue queue{VK_NULL_HANDLE};
            vkGetDeviceQueue(mHandle, queueFamilyIndex, queueIndex, &queue);
            mQueues[queueFamilyIndex].emplace_back(queue, queueFamilyIndex, queueIndex, presentSupported, queueFamilyProperty);
        }
    }
    
}

VulkanDevice::~VulkanDevice() {
    
}

VkDevice VulkanDevice::getHandle() {
    return mHandle;
}

std::vector<std::vector<VulkanQueue>> VulkanDevice::getQueues() {
    return mQueues;
}

VulkanQueue& VulkanDevice::getQueue(uint32_t queuefamilyIndex, uint32_t queueIndex) {
    return mQueues[queuefamilyIndex][queueIndex];
}

VulkanQueue& VulkanDevice::getQueueByFlags(VkQueueFlags requiredQueueFlags, uint32_t queueIndex) {
    for(uint32_t queueFamilyIndex = 0; queueFamilyIndex < mQueues.size(); ++queueFamilyIndex) {
        VulkanQueue& firstQueue = mQueues[queueFamilyIndex][0];
        VkQueueFlags queueFlags = firstQueue.getProperties().queueFlags;
        uint32_t queueCount = firstQueue.getProperties().queueCount;
        if(((queueFlags & requiredQueueFlags) == requiredQueueFlags) && queueIndex < queueCount) {
            return mQueues[queueFamilyIndex][queueIndex];
        }
    }
    throw std::runtime_error("Queue not found");
}

VulkanQueue& VulkanDevice::getSuitableGraphicsQueue() {
    for(uint32_t queueFamilyIndex = 0; queueFamilyIndex < mQueues.size(); ++queueFamilyIndex) {
        VulkanQueue& firstQueue = mQueues[queueFamilyIndex][0];
        uint32_t queueCount = firstQueue.getProperties().queueCount;
        if(firstQueue.getCanPresent() && queueCount > 0) {
            return mQueues[queueFamilyIndex][0];
        }
    }
    return getQueueByFlags(VK_QUEUE_GRAPHICS_BIT, 0);
}

bool VulkanDevice::isExtensionSupported(const std::string& requestedExtension) {
    return std::find_if(mDeviceExtensions.begin(), mDeviceExtensions.end(),
                        [requestedExtension](auto &deviceExtension) {
                            return std::strcmp(deviceExtension.extensionName, requestedExtension.c_str()) == 0;
                        }) != mDeviceExtensions.end();
}


} // namespace phatom

#ifndef PHATOM_VULKANCONFIG_H
#define PHATOM_VULKANCONFIG_H

#include <vector>
#include <unordered_map>
#include <vulkan/vulkan.h>

namespace phatom {

#ifdef NDEBUG
const bool kEnableDebug = false;
#else
const bool kEnableDebug = true;
#endif

const std::vector<const char*> kValidationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> kDeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

std::unordered_map<const char *, bool> DeviceExtensions = {{VK_KHR_SWAPCHAIN_EXTENSION_NAME, true}};

} // namespace phatom


#endif //PHATOM_VULKANCONFIG_H

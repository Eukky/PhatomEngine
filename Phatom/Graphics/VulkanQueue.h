#ifndef PHATOM_VULKANQUEUE_H
#define PHATOM_VULKANQUEUE_H

#include <vulkan/vulkan.h>

namespace phatom {
class VulkanQueue {
public:
    VulkanQueue(VkQueue hanlde, uint32_t familyIndex, uint32_t index, VkBool32 canPresent, VkQueueFamilyProperties properties);
    ~VulkanQueue();
    
    VkQueue getHandle();
    uint32_t getFamilyIndex();
    uint32_t getIndex();
    VkBool32 getCanPresent();
    VkQueueFamilyProperties getProperties();
private:
    VkQueue mHandle;
    uint32_t mFamilyIndex;
    uint32_t mIndex;
    VkBool32 mCanPresent;
    VkQueueFamilyProperties mProperties;
};
} // namespace phatom 


#endif //PHATOM_VULKANQUEUE_H

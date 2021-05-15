#include "VulkanQueue.h"

namespace phatom {
VulkanQueue::VulkanQueue(VkQueue handle, uint32_t familyIndex, uint32_t index, VkBool32 canPresent, VkQueueFamilyProperties properties) :
mHandle(handle),
mFamilyIndex(familyIndex),
mIndex(index),
mCanPresent(canPresent),
mProperties(properties){
}

VulkanQueue::~VulkanQueue() {
    
}

VkQueue VulkanQueue::getHandle() {
    return mHandle;
}

uint32_t VulkanQueue::getFamilyIndex() {
    return mFamilyIndex;
}

uint32_t VulkanQueue::getIndex() {
    return mIndex;
}

VkBool32 VulkanQueue::getCanPresent() {
    return mCanPresent;
}

VkQueueFamilyProperties VulkanQueue::getProperties() {
    return mProperties;
}

} // namespace phatom 

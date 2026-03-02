#include <renderer/sync.hpp>

VkSemaphore create_semaphore(const Device& device) {
    VkSemaphore semaphore;

    VkSemaphoreCreateInfo semaphore_info {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };

    if(vkCreateSemaphore(device.logical, &semaphore_info, nullptr, &semaphore) != VK_SUCCESS) {
        throw std::runtime_error("failed to create semaphore");
    }

    return semaphore;
}

VkFence create_fence(const Device& device) {
    VkFence fence;

    VkFenceCreateInfo fence_info {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    if(vkCreateFence(device.logical, &fence_info, nullptr, &fence) != VK_SUCCESS) {
        throw std::runtime_error("failed to create fence");
    }

    return fence;
}

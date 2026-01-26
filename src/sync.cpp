#include "sync.hpp"

Sync create_sync_objects(VkDevice device) {
    VkSemaphore semaphore0;
    VkSemaphore semaphore1;
    VkFence fence;

    VkSemaphoreCreateInfo semaphore_info {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };

    VkFenceCreateInfo fence_info {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    if(vkCreateSemaphore(device, &semaphore_info, nullptr, &semaphore0) != VK_SUCCESS ||
       vkCreateSemaphore(device, &semaphore_info, nullptr, &semaphore1) != VK_SUCCESS ||
       vkCreateFence(device, &fence_info, nullptr, &fence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create semaphores");
    }

    return Sync {
        .semaphores = { semaphore0, semaphore1 },
        .fences = { fence }
    };
}

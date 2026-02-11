#include <renderer/sync.hpp>

Sync create_sync_objects(const Device& device) {
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

    if(vkCreateSemaphore(device.logical.handle, &semaphore_info, nullptr, &semaphore0) != VK_SUCCESS ||
       vkCreateSemaphore(device.logical.handle, &semaphore_info, nullptr, &semaphore1) != VK_SUCCESS ||
       vkCreateFence(device.logical.handle, &fence_info, nullptr, &fence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create semaphores");
    }

    return Sync {
        .semaphores = { semaphore0, semaphore1 },
        .fences = { fence }
    };
}

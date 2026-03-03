#include <renderer/semaphore.hpp>

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

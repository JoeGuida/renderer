#include "command.hpp"

#include <stdexcept>

VkCommandPool create_command_pool(VkDevice device, uint32_t graphics_queue_family) {
    VkCommandPoolCreateInfo pool_info {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = graphics_queue_family
    };

    VkCommandPool command_pool;
    if(vkCreateCommandPool(device, &pool_info, nullptr, &command_pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool");
    }

    return command_pool;
}

VkCommandBuffer create_command_buffer(VkDevice device, VkCommandPool command_pool) {
    VkCommandBufferAllocateInfo alloc_info {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = command_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    VkCommandBuffer command_buffer;
    if(vkAllocateCommandBuffers(device, &alloc_info, &command_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers");
    }

    return command_buffer;
}

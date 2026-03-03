#include <renderer/fence.hpp>

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

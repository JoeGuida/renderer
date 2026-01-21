#ifndef RENDERER_SYNC_HPP
#define RENDERER_SYNC_HPP

#include <vector>

#include <vulkan/vulkan.hpp>

struct Sync {
    std::vector<VkSemaphore> semaphores;
    std::vector<VkFence> fences;
};

#endif

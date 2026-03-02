#ifndef RENDERER_SYNC_HPP
#define RENDERER_SYNC_HPP

#include <vulkan/vulkan.hpp>

#include "device.hpp"

VkSemaphore create_semaphore(const Device& device);
VkFence create_fence(const Device& device);

#endif

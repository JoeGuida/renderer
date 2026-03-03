#ifndef RENDERER_FENCE_HPP
#define RENDERER_FENCE_HPP

#include <vulkan/vulkan.hpp>

#include <renderer/device.hpp>

VkFence create_fence(const Device& device);

#endif

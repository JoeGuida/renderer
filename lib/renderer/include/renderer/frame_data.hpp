#ifndef RENDERER_FRAME_DATA_HPP
#define RENDERER_FRAME_DATA_HPP

#include <vulkan/vulkan.hpp>

#include <renderer/device.hpp>

struct FrameData {
    VkSemaphore begin;
    VkSemaphore in_flight;
    VkFence fence;
};

void destroy(const Device& device, const FrameData& frame_data);

#endif

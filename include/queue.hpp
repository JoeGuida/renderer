#ifndef RENDERER_QUEUE_HPP
#define RENDERER_QUEUE_HPP

#include <cstdint>
#include <optional>

#include <vulkan/vulkan.hpp>

struct QueueFamily {
    uint32_t graphics;
    uint32_t presentation;
};

struct RenderQueue {
    VkQueue graphics;
    VkQueue presentation;
};

std::optional<QueueFamily> get_queue_family(VkPhysicalDevice physical_device, VkSurfaceKHR surface);

#endif

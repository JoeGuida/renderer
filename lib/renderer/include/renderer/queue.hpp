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

RenderQueue get_render_queue(VkDevice logical_device, uint32_t graphics_queuee_id, uint32_t presentation_queue_id);
std::optional<QueueFamily> get_queue_family(VkPhysicalDevice physical_device, VkSurfaceKHR surface);

#endif

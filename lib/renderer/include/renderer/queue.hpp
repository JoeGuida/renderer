#ifndef RENDERER_QUEUE_HPP
#define RENDERER_QUEUE_HPP

#include <cstdint>
#include <optional>

#include <vulkan/vulkan.hpp>

#include "device.hpp"

struct RenderQueue {
    VkQueue graphics;
    VkQueue presentation;
};

RenderQueue get_render_queue(const Device& device, uint32_t graphics_queuee_id, uint32_t presentation_queue_id);
std::optional<QueueFamily> get_queue_family(const PhysicalDevice& device, VkSurfaceKHR surface);

#endif

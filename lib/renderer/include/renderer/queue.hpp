#ifndef RENDERER_QUEUE_HPP
#define RENDERER_QUEUE_HPP

#ifdef VULKAN

#include <cstdint>
#include <optional>

#include <vulkan/vulkan.hpp>

#include <renderer/device.hpp>

struct RenderQueue {
    VkQueue graphics;
    VkQueue presentation;
};

RenderQueue get_render_queue(const Device& device, uint32_t graphics_queuee_id, uint32_t presentation_queue_id);
std::optional<QueueFamily> get_queue_family(VkPhysicalDevice device, VkSurfaceKHR surface);

#endif

#endif

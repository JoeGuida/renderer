#ifndef RENDERER_DEVICE_HPP
#define RENDERER_DEVICE_HPP

#ifdef VULKAN

#include <vulkan/vulkan.hpp>

#include <renderer/renderer_extensions.hpp>
#include <renderer/queue_family.hpp>

struct Device {
    VkPhysicalDevice physical;
    VkDevice logical;
};

int score_device(VkPhysicalDevice device);
VkDevice create_logical_device(VkPhysicalDevice device, QueueFamily queue_family, const RendererExtensions& extensions);
VkPhysicalDevice create_physical_device(VkInstance instance, VkSurfaceKHR surface, const RendererExtensions& extensions);

#endif

#endif

#ifndef RENDERER_DEVICE_HPP
#define RENDERER_DEVICE_HPP

#include <vulkan/vulkan.hpp>

#include "extension.hpp"
#include "queue.hpp"

struct VulkanDevice {
    VkPhysicalDevice physical;
    VkDevice logical;
};

int score_device(VkPhysicalDevice physical_device);
VkDevice create_logical_device(VkPhysicalDevice device, QueueFamily queue_family, const std::vector<const char*>& device_extensions);
VkPhysicalDevice create_physical_device(VkInstance instance, VkSurfaceKHR surface, const RendererExtensions& extensions);

#endif

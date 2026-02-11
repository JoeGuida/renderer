#ifndef RENDERER_DEVICE_HPP
#define RENDERER_DEVICE_HPP

#include <vulkan/vulkan.hpp>

#include "extension.hpp"
#include "queue.hpp"

struct PhysicalDevice {
    VkPhysicalDevice handle;
};

struct LogicalDevice {
    VkDevice handle;
};

struct Device {
    PhysicalDevice physical;
    LogicalDevice logical;
};

int score_device(const PhysicalDevice& device);
LogicalDevice create_logical_device(const PhysicalDevice& device, QueueFamily queue_family, const RendererExtensions& extensions);
PhysicalDevice create_physical_device(VkInstance instance, VkSurfaceKHR surface, const RendererExtensions& extensions);

#endif

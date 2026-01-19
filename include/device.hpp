#ifndef RENDERER_DEVICE_HPP
#define RENDERER_DEVICE_HPP

#include <vulkan/vulkan.hpp>

struct VulkanDevice {
    VkPhysicalDevice physical;
    VkDevice logical;
};

#endif

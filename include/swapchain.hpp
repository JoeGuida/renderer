#ifndef RENDERER_SWAPCHAIN_HPP
#define RENDERER_SWAPCHAIN_HPP

#include <vector>

#include <platform/platform.hpp>

#include <vulkan/vulkan.hpp>

#include "device.hpp"

struct Swapchain {
    VkSwapchainKHR handle;
    std::vector<VkImage> images;
    std::vector<VkImageView> image_views;
    VkExtent2D extent;
    VkFormat image_format;
    VkPresentModeKHR present_mode;
    std::vector<VkFramebuffer> framebuffers;
};

struct SwapchainSupportInfo {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
};

Swapchain create_swapchain(HWND hwnd, VulkanDevice device, VkSurfaceKHR surface);
SwapchainSupportInfo query_swapchain_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface);

#endif

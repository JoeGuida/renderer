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
VkSurfaceFormatKHR choose_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats, VkFormat format, VkColorSpaceKHR color_space);
VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes, VkPresentModeKHR mode);
void create_framebuffers(VkDevice device, Swapchain& swapchain, VkRenderPass render_pass);
void create_image_views(VkDevice device, Swapchain& swapchain);

#endif

#ifndef RENDERER_SWAPCHAIN_HPP
#define RENDERER_SWAPCHAIN_HPP

#include <vector>

#include <platform/platform.hpp>

#include <vulkan/vulkan.hpp>

#include "device.hpp"

struct Extent {
    uint32_t width;
    uint32_t height;
};

enum class ImageFormat : uint32_t {
    RGBA8_UNORM = VK_FORMAT_R8G8B8A8_UNORM
};

enum class ColorSpace : uint32_t {
    SRGB_NONLINEAR = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
};

enum class PresentMode : uint32_t {
    MAILBOX = VK_PRESENT_MODE_MAILBOX_KHR
};

struct SurfaceFormat {
    ImageFormat image_format;
    ColorSpace color_space;
};

struct Swapchain {
    VkSwapchainKHR handle;
    std::vector<VkImage> images;
    std::vector<VkImageView> image_views;
    Extent extent;
    VkSurfaceFormatKHR surface_format;
    VkPresentModeKHR present_mode;
    std::vector<VkFramebuffer> framebuffers;
    VkRenderPass render_pass;
};

struct SwapchainSupportInfo {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
};

void create_swapchain(HWND hwnd, const Device& device, VkSurfaceKHR surface, Swapchain& swapchain, VkSurfaceFormatKHR format, VkPresentModeKHR present_mode, Swapchain* old_swapchain);
SwapchainSupportInfo query_swapchain_support(const PhysicalDevice& device, VkSurfaceKHR surface);
VkSurfaceFormatKHR choose_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats, VkFormat format, VkColorSpaceKHR color_space);
VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes, VkPresentModeKHR mode);
void create_framebuffers(const Device& device, Swapchain& swapchain, VkRenderPass render_pass);
void create_image_views(const Device& device, Swapchain& swapchain);
void destroy_swapchain(Swapchain& swapchain, const Device& device);
void rebuild_swapchain(HWND hwnd, const Device& device, VkSurfaceKHR surface, Swapchain& swapchain, Swapchain& old_swapchain);

#endif

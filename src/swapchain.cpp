#include "swapchain.hpp"

Swapchain create_swapchain(HWND hwnd, VulkanDevice device, VkSurfaceKHR surface) {
    VkFormat surface_format = VK_FORMAT_B8G8R8A8_SRGB;
    VkColorSpaceKHR color_space = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    VkPresentModeKHR present_mode = VK_PRESENT_MODE_MAILBOX_KHR;

    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.physical, surface, &capabilities);

    RECT client_rect;
    GetClientRect(hwnd, &client_rect);
    VkExtent2D extent = { 
        static_cast<uint32_t>(client_rect.right - client_rect.left),
        static_cast<uint32_t>(client_rect.bottom - client_rect.top)
    };

    uint32_t image_count = 2;

    VkSwapchainCreateInfoKHR create_info {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface,
        .minImageCount = image_count,
        .imageFormat = surface_format,
        .imageColorSpace = color_space,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = present_mode,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    };

    VkSwapchainKHR swapchain;
    if(vkCreateSwapchainKHR(device.logical, &create_info, nullptr, &swapchain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swapchain");
    }

    std::vector<VkImage> swapchain_images;
    vkGetSwapchainImagesKHR(device.logical, swapchain, &image_count, nullptr);
    swapchain_images.resize(image_count);
    vkGetSwapchainImagesKHR(device.logical, swapchain, &image_count, swapchain_images.data());

    return Swapchain {
        .handle = swapchain,
        .images = swapchain_images,
        .extent = extent,
        .image_format = surface_format
    };
}

SwapchainSupportInfo query_swapchain_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
    SwapchainSupportInfo swapchain_support_info;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &swapchain_support_info.capabilities);

    uint32_t format_count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr);
    if(format_count != 0) {
        swapchain_support_info.formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, swapchain_support_info.formats.data());
    }

    uint32_t present_mode_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr);
    if(present_mode_count != 0) {
        swapchain_support_info.present_modes.resize(present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, swapchain_support_info.present_modes.data());
    }

    return swapchain_support_info;
}

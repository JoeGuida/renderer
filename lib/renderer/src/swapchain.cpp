#include <renderer/swapchain.hpp>

#include <algorithm>

void create_swapchain(HWND hwnd, Device device, VkSurfaceKHR surface, Swapchain& swapchain, VkFormat format = VK_FORMAT_B8G8R8A8_SRGB, VkColorSpaceKHR color_space = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR, VkPresentModeKHR present_mode = VK_PRESENT_MODE_MAILBOX_KHR, Swapchain* old_swapchain = nullptr) {
    SwapchainSupportInfo info = query_swapchain_support(device.physical.handle, surface);
    VkSurfaceFormatKHR surface_format = choose_surface_format(info.formats, format, color_space);
    swapchain.image_format = surface_format.format;
    swapchain.color_space = surface_format.colorSpace;
    swapchain.present_mode = choose_present_mode(info.present_modes, present_mode);


    RECT client_rect;
    GetClientRect(hwnd, &client_rect);
    uint32_t rect_width = static_cast<uint32_t>(client_rect.right - client_rect.left);
    uint32_t rect_height = static_cast<uint32_t>(client_rect.bottom - client_rect.top);

    if(info.capabilities.currentExtent.width == UINT32_MAX) {
        uint32_t width = std::clamp(rect_width, info.capabilities.minImageExtent.width, info.capabilities.maxImageExtent.width);
        uint32_t height = std::clamp(rect_height, info.capabilities.minImageExtent.height, info.capabilities.maxImageExtent.height);
        swapchain.extent = { width, height };
    }
    else {
        swapchain.extent = info.capabilities.currentExtent;
    }

    if(swapchain.extent.width == 0 or swapchain.extent.height == 0) {
        return;
    }

    uint32_t image_count = info.capabilities.minImageCount + 1;
    if(info.capabilities.maxImageCount > 0 && image_count > info.capabilities.maxImageCount) {
        image_count = info.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface,
        .minImageCount = image_count,
        .imageFormat = swapchain.image_format,
        .imageColorSpace = swapchain.color_space,
        .imageExtent = swapchain.extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = info.capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = swapchain.present_mode,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    };

    if(old_swapchain) {
        create_info.oldSwapchain = old_swapchain->handle;
    }
    else {
        create_info.oldSwapchain = VK_NULL_HANDLE;
    }

    if(vkCreateSwapchainKHR(device.logical.handle, &create_info, nullptr, &swapchain.handle) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swapchain");
    }

    vkGetSwapchainImagesKHR(device.logical.handle, swapchain.handle, &image_count, nullptr);
    swapchain.images.resize(image_count);
    vkGetSwapchainImagesKHR(device.logical.handle, swapchain.handle, &image_count, swapchain.images.data());
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

VkSurfaceFormatKHR choose_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats, VkFormat format, VkColorSpaceKHR color_space) {
    for(const auto& available_format : available_formats) {
        if(available_format.format == format && available_format.colorSpace == color_space) {
            return available_format;
        }
    }

    return available_formats[0];
}

VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes, VkPresentModeKHR mode) {
    for(const auto& available_mode : available_present_modes) {
        if(available_mode == mode) {
            return mode;
        }
    }

    return available_present_modes[0];
}

void create_framebuffers(VkDevice device, Swapchain& swapchain, VkRenderPass render_pass) {
    swapchain.framebuffers.resize(swapchain.image_views.size());

    for(size_t i = 0; i < swapchain.image_views.size(); i++) {
        VkImageView attachments[] = { swapchain.image_views[i] };

        VkFramebufferCreateInfo framebuffer_info {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = render_pass,
            .attachmentCount = 1,
            .pAttachments = attachments,
            .width = swapchain.extent.width,
            .height = swapchain.extent.height,
            .layers = 1
        };

        if(vkCreateFramebuffer(device, &framebuffer_info, nullptr, &swapchain.framebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer");
        }
    }

    swapchain.render_pass = render_pass;
}

void create_image_views(VkDevice device, Swapchain& swapchain) {
    if(swapchain.images.size() > 0) {
        swapchain.image_views.resize(swapchain.images.size());
    }

    for(size_t i = 0; i < swapchain.images.size(); i++) {
        VkImageViewCreateInfo create_info{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = swapchain.images[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = swapchain.image_format,
            .components {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };

        if(vkCreateImageView(device, &create_info, nullptr, &swapchain.image_views[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views");
        }
    }
}

void destroy_swapchain(Swapchain& swapchain, VkDevice device) {
    for(auto& framebuffer : swapchain.framebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }

    for(auto& image_view : swapchain.image_views) {
        vkDestroyImageView(device, image_view, nullptr);
    }

    vkDestroySwapchainKHR(device, swapchain.handle, nullptr);
}

void rebuild_swapchain(HWND hwnd, Device device, VkSurfaceKHR surface, Swapchain& swapchain, Swapchain& old_swapchain) {
    vkDeviceWaitIdle(device.logical.handle);
    create_swapchain(hwnd, device, surface, swapchain, old_swapchain.image_format, old_swapchain.color_space, old_swapchain.present_mode, &old_swapchain);

    if(!swapchain.images.empty()) {
        create_image_views(device.logical.handle, swapchain);
        create_framebuffers(device.logical.handle, swapchain, old_swapchain.render_pass);
    }

    destroy_swapchain(old_swapchain, device.logical.handle);
}

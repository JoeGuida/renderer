#include "surface.hpp"

VkSurfaceKHR create_window_surface(VkInstance vk_instance, PlatformWindow* window, HINSTANCE instance) {
    VkSurfaceKHR surface;

    VkWin32SurfaceCreateInfoKHR create_info {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .hinstance = instance,
        .hwnd = window->hwnd
    };

    if(vkCreateWin32SurfaceKHR(vk_instance, &create_info, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface");
    }

    return surface;
}

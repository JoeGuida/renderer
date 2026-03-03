#ifndef RENDERER_SURFACE_HPP
#define RENDERER_SURFACE_HPP

#ifdef VULKAN

#include <platform/platform.hpp>
#include <platform/windows/win32_window.hpp>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_win32.h>


struct Surface {
    VkSurfaceKHR handle;
};

Surface create_window_surface(VkInstance vk_instance, PlatformWindow* window, HINSTANCE instance);

#endif

#endif

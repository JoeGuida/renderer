#ifndef RENDERER_SURFACE_HPP
#define RENDERER_SURFACE_HPP

#include <vulkan/vulkan.hpp>

#ifdef WINDOWS

#include <platform/platform.hpp>
#include <vulkan/vulkan_win32.h>
#include <platform/windows/win32_window.hpp>

#endif

VkSurfaceKHR create_window_surface(VkInstance vk_instance, PlatformWindow* window, HINSTANCE instance);

#endif

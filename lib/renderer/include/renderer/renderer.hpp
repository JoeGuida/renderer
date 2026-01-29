#ifndef RENDERER_RENDERER_HPP
#define RENDERER_RENDERER_HPP

#include <string>

#include <vulkan/vulkan.hpp>

#if defined(_WIN32) || defined(_WIN64)
#include <platform/windows/win32_window.hpp>
#endif

#include "context.hpp"

struct Renderer {

};

void draw(VkContext context, PlatformWindow* window);
void initialize(Renderer& renderer);
std::expected<VkContext, std::string> init_renderer(Renderer& renderer, PlatformWindow* window, HINSTANCE instance, const RendererExtensions& extensions);

#endif

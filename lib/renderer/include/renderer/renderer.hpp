#ifndef RENDERER_RENDERER_HPP
#define RENDERER_RENDERER_HPP

#include <string>

#include <vulkan/vulkan.hpp>

#ifdef WINDOWS
#include <platform/windows/win32_window.hpp>
#elifdef APPLE
#include <platform/mac/mac_window.hpp>
#endif

#include "context.hpp"
#include "extension.hpp"

struct Renderer {

};

void draw(VkContext& context, PlatformWindow* window);
void initialize(Renderer& renderer);

#ifdef WINDOWS
std::expected<VkContext, std::string> init_renderer(Renderer& renderer, PlatformWindow* window, HINSTANCE instance, const RendererFeatures& features);
#elifdef APPLE
std::expected<VkContext, std::string> init_renderer(Renderer& renderer, PlatformWindow* window, const RendererFeatures& features);
#endif

#endif

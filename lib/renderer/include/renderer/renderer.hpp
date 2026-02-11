#ifndef RENDERER_RENDERER_HPP
#define RENDERER_RENDERER_HPP

#include <string>

#include <vulkan/vulkan.hpp>

#include <platform/windows/win32_window.hpp>

#include "context.hpp"
#include "extension.hpp"

struct Renderer {

};

void draw(VkContext& context, PlatformWindow* window);
void initialize(Renderer& renderer);

std::expected<VkContext, std::string> init_renderer(Renderer& renderer, PlatformWindow* window, HINSTANCE instance, const RendererFeatures& features);

#endif

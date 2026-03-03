#ifndef RENDERER_RENDERER_HPP
#define RENDERER_RENDERER_HPP

#include <expected>
#include <string>

#include <platform/windows/win32_window.hpp>

#include <renderer/context.hpp>
#include <renderer/extension.hpp>

struct Renderer {

};

void draw(Context& context, PlatformWindow* window);
void initialize(Renderer& renderer);

std::expected<Context, std::string> init_renderer(Renderer& renderer, PlatformWindow* window, HINSTANCE instance, const RendererFeatures& features);

#endif

#ifndef RENDERER_RENDERER_HPP
#define RENDERER_RENDERER_HPP

#include <expected>
#include <string>

#include "window/window.hpp"

struct Renderer {

};

std::expected<void, std::string> initialize_opengl(PlatformWindow* handle);

#endif


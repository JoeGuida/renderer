#ifndef GL_RENDERER_CONTEXT_HPP
#define GL_RENDERER_CONTEXT_HPP

#include <memory>

#include "input.hpp"
#include "renderer.hpp"

struct Context {
    std::unique_ptr<Input> input;
    std::unique_ptr<Renderer> renderer;
};

#endif

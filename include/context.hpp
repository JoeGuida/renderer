#ifndef GL_RENDERER_CONTEXT_HPP
#define GL_RENDERER_CONTEXT_HPP

#include "input.hpp"
#include "renderer.hpp"

struct Context {
    Input* input;
    Renderer* renderer;
};

#endif

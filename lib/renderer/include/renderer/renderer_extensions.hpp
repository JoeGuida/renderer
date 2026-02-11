#ifndef RENDERER_RENDERER_EXTENSIONS_HPP
#define RENDERER_RENDERER_EXTENSIONS_HPP

#include <vector>

struct RendererExtensions {
    std::vector<const char*> instance;
    std::vector<const char*> device;
    std::vector<const char*> validation;
};

#endif

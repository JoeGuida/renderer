#ifndef RENDERER_VIEWPORT_SCISSOR_HPP
#define RENDERER_VIEWPORT_SCISSOR_HPP

#ifdef VULKAN

#include <vulkan/vulkan.hpp>

struct ViewportScissor {
    VkViewport viewport;
    VkRect2D scissor;
};

#endif

#endif

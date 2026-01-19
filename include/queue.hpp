#ifndef RENDERER_QUEUE_HPP
#define RENDERER_QUEUE_HPP

#include <cstdint>
#include <vulkan/vulkan.hpp>

struct QueueFamily {
    uint32_t graphics;
    uint32_t presentation;
};

struct RenderQueue {
    VkQueue graphics;
    VkQueue presentation;
};

#endif

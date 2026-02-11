#ifndef RENDERER_QUEUE_FAMILY_HPP
#define RENDERER_QUEUE_FAMILY_HPP

#include <cstdint>

struct QueueFamily {
    uint32_t graphics;
    uint32_t presentation;
};

#endif

#ifndef RENDERER_INSTANCE_HPP
#define RENDERER_INSTANCE_HPP

#ifdef VULKAN

#include <renderer/renderer_extensions.hpp>

#include <vulkan/vulkan.hpp>

struct Instance {
    VkInstance handle;
};

Instance create_instance(const RendererExtensions& extensions);

#endif

#endif

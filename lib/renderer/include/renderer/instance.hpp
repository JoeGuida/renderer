#ifndef RENDERER_INSTANCE_HPP
#define RENDERER_INSTANCE_HPP

#include <vulkan/vulkan.hpp>

#include <renderer/renderer_extensions.hpp>

VkInstance create_instance(const RendererExtensions& extensions);

#endif

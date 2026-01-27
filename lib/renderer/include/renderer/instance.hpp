#ifndef RENDERER_INSTANCE_HPP
#define RENDERER_INSTANCE_HPP

#include <vulkan/vulkan.hpp>

#include "extension.hpp"

VkInstance create_instance(const RendererExtensions& extensions);

#endif

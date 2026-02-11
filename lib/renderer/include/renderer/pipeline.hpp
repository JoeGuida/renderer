#ifndef RENDERER_PIPELINE_HPP
#define RENDERER_PIPELINE_HPP

#include <vulkan/vulkan.hpp>

#include "device.hpp"

VkPipeline create_graphics_pipeline(const Device& device, VkExtent2D extent, VkRenderPass render_pass);

#endif

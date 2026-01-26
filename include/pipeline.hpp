#ifndef RENDERER_PIPELINE_HPP
#define RENDERER_PIPELINE_HPP

#include <vulkan/vulkan.hpp>

VkPipeline create_graphics_pipeline(VkDevice device, VkExtent2D extent, VkRenderPass render_pass);

#endif

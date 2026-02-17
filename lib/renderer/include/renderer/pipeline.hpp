#ifndef RENDERER_PIPELINE_HPP
#define RENDERER_PIPELINE_HPP

#include <vulkan/vulkan.hpp>

#include "device.hpp"
#include "swapchain.hpp"

VkPipeline create_graphics_pipeline(const Device& device, Extent extent, VkRenderPass render_pass);

#endif

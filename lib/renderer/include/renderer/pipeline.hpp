#ifndef RENDERER_PIPELINE_HPP
#define RENDERER_PIPELINE_HPP

#ifdef VULKAN

#include <vulkan/vulkan.hpp>

#include <renderer/device.hpp>
#include <renderer/swapchain.hpp>

struct Pipeline {
    VkPipeline handle;
};

Pipeline create_graphics_pipeline(const Device& device, Extent extent, VkRenderPass render_pass);

#endif

#endif

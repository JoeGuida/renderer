#ifndef RENDERER_COMMAND_HPP
#define RENDERER_COMMAND_HPP

#include <cstdint>

#include <vulkan/vulkan.hpp>

#include "device.hpp"
#include "swapchain.hpp"

struct Command {
    VkCommandPool pool;
    VkCommandBuffer buffer;
    std::vector<VkRenderPass> render_passes;
};

VkCommandPool create_command_pool(const Device& device, uint32_t graphics_queue_family);
VkCommandBuffer create_command_buffer(const Device& device, VkCommandPool command_pool);
void record_command_buffer(Swapchain& swapchain, uint32_t framebuffer_index, VkCommandBuffer command_buffer, VkRenderPass render_pass, VkPipeline graphics_pipeline);

#endif

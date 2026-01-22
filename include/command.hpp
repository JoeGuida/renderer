#ifndef RENDERER_COMMAND_HPP
#define RENDERER_COMMAND_HPP

#include <cstdint>

#include <vulkan/vulkan.hpp>

struct Command {
    VkCommandPool pool;
    VkCommandBuffer buffer;
    std::vector<VkRenderPass> render_passes;
};

VkCommandPool create_command_pool(VkDevice device, uint32_t graphics_queue_family);
VkCommandBuffer create_command_buffer(VkDevice device, VkCommandPool command_pool);

#endif

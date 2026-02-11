#ifndef RENDERER_CONTEXT_HPP
#define RENDERER_CONTEXT_HPP

#include <vector>

#include <vulkan/vulkan.hpp>

#include "device.hpp"
#include "queue.hpp"
#include "swapchain.hpp"
#include "sync.hpp"

struct Context {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    Device device;
    VkSurfaceKHR surface;
    RenderQueue queue;
    Swapchain swapchain;
    Swapchain old_swapchain;
    VkRenderPass render_pass;
    VkPipeline pipeline;
    VkCommandPool command_pool;
    VkCommandBuffer command_buffer;
    Sync sync;
};

void destroy_context(const Context& context);

#endif

#ifndef RENDERER_CONTEXT_HPP
#define RENDERER_CONTEXT_HPP

#include <vector>

#include <vulkan/vulkan.hpp>

#include "device.hpp"
#include "queue.hpp"
#include "swapchain.hpp"

struct VkContext {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    VkPhysicalDevice physical_device;
    VulkanDevice device;
    VkSurfaceKHR surface;
    RenderQueue queue;
    Swapchain swapchain;
    VkRenderPass render_pass;
    VkPipeline pipeline;
    VkPipelineLayout pipeline_layout;
    VkCommandPool command_pool;
    VkCommandBuffer command_buffer;

    std::vector<VkSemaphore> semaphores;
    std::vector<VkFence> fences;
};

void destroy_context(VkContext context);

#endif

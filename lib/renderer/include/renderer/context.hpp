#ifndef RENDERER_CONTEXT_HPP
#define RENDERER_CONTEXT_HPP

#include <vector>

#include <vulkan/vulkan.hpp>

#include "command.hpp"
#include "device.hpp"
#include "frame_data.hpp"
#include "queue.hpp"
#include "swapchain.hpp"

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
    Command command;
    FrameData frame_data;
};

void destroy_context(const Context& context);

#endif

#ifndef RENDERER_CONTEXT_HPP
#define RENDERER_CONTEXT_HPP

#include <renderer/command.hpp>
#include <renderer/debug_messenger.hpp>
#include <renderer/device.hpp>
#include <renderer/frame_data.hpp>
#include <renderer/instance.hpp>
#include <renderer/pipeline.hpp>
#include <renderer/queue.hpp>
#include <renderer/surface.hpp>
#include <renderer/swapchain.hpp>

struct Context {
    Instance instance;
    DebugMessenger debug_messenger;
    Device device;
    Surface surface;
    RenderQueue queue;
    Swapchain swapchain;
    Swapchain old_swapchain;
    Pipeline pipeline;
    Command command;
    FrameData frame_data;
};

void destroy(const Context& context);

#endif

#include <renderer/context.hpp>

#include <renderer/debug_messenger.hpp>

void destroy_context(const Context& context) {
    vkDeviceWaitIdle(context.device.logical.handle);

    for(auto& semaphore : context.sync.semaphores) {
        vkDestroySemaphore(context.device.logical.handle, semaphore, nullptr);
    }

    for(auto& fence : context.sync.fences) {
        vkDestroyFence(context.device.logical.handle, fence, nullptr);
    }

    vkDestroyCommandPool(context.device.logical.handle, context.command_pool, nullptr);

    for(auto& framebuffer : context.swapchain.framebuffers) {
        vkDestroyFramebuffer(context.device.logical.handle, framebuffer, nullptr);
    }

    vkDestroyPipeline(context.device.logical.handle, context.pipeline, nullptr);
    vkDestroyRenderPass(context.device.logical.handle, context.render_pass, nullptr);

    for(auto& image_view : context.swapchain.image_views) {
        vkDestroyImageView(context.device.logical.handle, image_view, nullptr);
    }

    vkDestroySwapchainKHR(context.device.logical.handle, context.swapchain.handle, nullptr);
    vkDestroySurfaceKHR(context.instance, context.surface, nullptr);
    vkDestroyDevice(context.device.logical.handle, nullptr);
    destroy_debug_utils_messenger_ext(context.instance, context.debug_messenger);
    vkDestroyInstance(context.instance, nullptr);
}

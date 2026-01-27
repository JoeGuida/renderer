#include <renderer/context.hpp>

#include <renderer/debug_messenger.hpp>

void destroy_context(VkContext context) {
    vkDeviceWaitIdle(context.device.logical);

    for(auto& semaphore : context.sync.semaphores) {
        vkDestroySemaphore(context.device.logical, semaphore, nullptr);
    }

    for(auto& fence : context.sync.fences) {
        vkDestroyFence(context.device.logical, fence, nullptr);
    }

    vkDestroyCommandPool(context.device.logical, context.command_pool, nullptr);

    for(auto& framebuffer : context.swapchain.framebuffers) {
        vkDestroyFramebuffer(context.device.logical, framebuffer, nullptr);
    }

    vkDestroyPipeline(context.device.logical, context.pipeline, nullptr);
    vkDestroyRenderPass(context.device.logical, context.render_pass, nullptr);

    for(auto& image_view : context.swapchain.image_views) {
        vkDestroyImageView(context.device.logical, image_view, nullptr);
    }

    vkDestroySwapchainKHR(context.device.logical, context.swapchain.handle, nullptr);
    vkDestroySurfaceKHR(context.instance, context.surface, nullptr);
    vkDestroyDevice(context.device.logical, nullptr);
    destroy_debug_utils_messenger_ext(context.instance, context.debug_messenger);
    vkDestroyInstance(context.instance, nullptr);
}

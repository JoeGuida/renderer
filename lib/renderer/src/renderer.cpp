#include <renderer/renderer.hpp>

#include <renderer/command.hpp>
#include <renderer/context.hpp>
#include <renderer/debug_messenger.hpp>
#include <renderer/device.hpp>
#include <renderer/extension.hpp>
#include <renderer/instance.hpp>
#include <renderer/pipeline.hpp>
#include <renderer/queue.hpp>
#include <renderer/render_pass.hpp>
#include <renderer/surface.hpp>
#include <renderer/swapchain.hpp>
#include <renderer/sync.hpp>

void draw(VkContext& context, PlatformWindow* window) {
    RECT client_rect;
    GetClientRect(window->hwnd, &client_rect);
    uint32_t rect_width = static_cast<uint32_t>(client_rect.right - client_rect.left);
    uint32_t rect_height = static_cast<uint32_t>(client_rect.bottom - client_rect.top);
    if(rect_width == 0 || rect_height == 0) {
        return;
    }

    vkWaitForFences(context.device.logical, 1, &context.sync.fences[0], VK_TRUE, UINT64_MAX);
    vkResetFences(context.device.logical, 1, &context.sync.fences[0]);

    uint32_t image_index;
    VkResult result = vkAcquireNextImageKHR(context.device.logical, context.swapchain.handle, UINT64_MAX, context.sync.semaphores[0], VK_NULL_HANDLE, &image_index);
    if(result == VK_ERROR_OUT_OF_DATE_KHR) {
        context.old_swapchain = context.swapchain;
        rebuild_swapchain(window->hwnd, context.device, context.surface, context.swapchain, context.old_swapchain);
        return;
    }
    else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("could not acquire swapchain image");
    }

    vkResetCommandBuffer(context.command_buffer, 0);
    record_command_buffer(context.swapchain, image_index, context.command_buffer, context.render_pass, context.pipeline);

    VkSemaphore wait_semaphores[] = { context.sync.semaphores[0] };
    VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkSemaphore signal_semaphores[] = { context.sync.semaphores[1] };

    VkSubmitInfo submit_info {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = wait_semaphores,
        .pWaitDstStageMask = wait_stages,
        .commandBufferCount = 1,
        .pCommandBuffers = &context.command_buffer,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = signal_semaphores
    };

    if(vkQueueSubmit(context.queue.graphics, 1, &submit_info, context.sync.fences[0]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer");
    }

    VkSwapchainKHR swapchains[] = { context.swapchain.handle };

    VkPresentInfoKHR present_info {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = signal_semaphores,
        .swapchainCount = 1,
        .pSwapchains = swapchains,
        .pImageIndices = &image_index,
        .pResults = nullptr
    };

    VkResult present = vkQueuePresentKHR(context.queue.presentation, &present_info);
    if(present == VK_ERROR_OUT_OF_DATE_KHR) {
        context.old_swapchain = context.swapchain;
        rebuild_swapchain(window->hwnd, context.device, context.surface, context.swapchain, context.old_swapchain);
        return;
    }
    else if(present != VK_SUCCESS && present != VK_SUBOPTIMAL_KHR) {
         throw std::runtime_error("could not acquire swapchain image");
    }
}

std::expected<VkContext, std::string> init_renderer(Renderer& renderer, PlatformWindow* window, HINSTANCE instance, const RendererExtensions& extensions) {
    VkContext context = {};

    if(!validation_layers_available(extensions)) {
        return std::unexpected("validation layers are not available!");
    }

    if(!instance_extensions_supported(extensions)) {
        return std::unexpected("requested instance extensions are not available");
    }

    context.instance = create_instance(extensions);
    context.debug_messenger = setup_debug_messenger(context.instance);
    context.surface = create_window_surface(context.instance, window, instance);
    context.device.physical = create_physical_device(context.instance, context.surface, extensions);

    auto queue_family = get_queue_family(context.device.physical, context.surface);
    if(!queue_family.has_value()) {
        return std::unexpected("queue_family not found");
    }

    context.device.logical = create_logical_device(context.device.physical, queue_family.value(), extensions.device);
    context.queue = get_render_queue(context.device.logical, queue_family.value().graphics, queue_family.value().presentation);
    Swapchain swapchain;
    create_swapchain(window->hwnd, context.device, context.surface, swapchain, VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR, VK_PRESENT_MODE_MAILBOX_KHR, nullptr);
    create_image_views(context.device.logical, swapchain);
    context.swapchain = swapchain;

    context.render_pass = create_render_pass(context.device.logical, swapchain.image_format);
    context.pipeline = create_graphics_pipeline(context.device.logical, swapchain.extent, context.render_pass);

    create_framebuffers(context.device.logical, context.swapchain, context.render_pass);

    context.command_pool = create_command_pool(context.device.logical, queue_family.value().graphics);
    context.command_buffer = create_command_buffer(context.device.logical, context.command_pool);
    context.sync = create_sync_objects(context.device.logical);

    return context;
}

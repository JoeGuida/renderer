#include <renderer/renderer.hpp>

void draw(Context& context, PlatformWindow* window) {
    auto [width, height] = get_window_size(window);
    if(width == 0 || height == 0) {
        return;
    }

    vkWaitForFences(context.device.logical, 1, &context.frame_data.fence, VK_TRUE, UINT64_MAX);
    vkResetFences(context.device.logical, 1, &context.frame_data.fence);

    uint32_t image_index;
    VkResult result = vkAcquireNextImageKHR(context.device.logical, context.swapchain.handle, UINT64_MAX, context.frame_data.begin, VK_NULL_HANDLE, &image_index);
    if(result == VK_ERROR_OUT_OF_DATE_KHR) {
        context.old_swapchain = context.swapchain;
        rebuild_swapchain(window->hwnd, context.device, context.surface, context.swapchain, context.old_swapchain);
        return;
    }
    else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("could not acquire swapchain image");
    }

    vkResetCommandBuffer(context.command.buffer, 0);
    record_command_buffer(context.swapchain, image_index, context.command.buffer, context.command.render_passes[0], context.pipeline.handle);

    VkSemaphore wait_semaphores[] = { context.frame_data.begin };
    VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkSemaphore signal_semaphores[] = { context.frame_data.in_flight };

    VkSubmitInfo submit_info {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = wait_semaphores,
        .pWaitDstStageMask = wait_stages,
        .commandBufferCount = 1,
        .pCommandBuffers = &context.command.buffer,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = signal_semaphores
    };

    if(vkQueueSubmit(context.queue.graphics, 1, &submit_info, context.frame_data.fence) != VK_SUCCESS) {
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

std::expected<Context, std::string> init_renderer(Renderer& renderer, PlatformWindow* window, HINSTANCE instance, const RendererFeatures& features) {
    Context context = {};

    RendererExtensions extensions = get_renderer_extensions(features);

    if(!validation_layers_available(extensions)) {
        return std::unexpected("validation layers are not available!");
    }

    if(!instance_extensions_supported(extensions)) {
        return std::unexpected("requested instance extensions are not available");
    }

    context.instance = create_instance(extensions);
    context.debug_messenger = setup_debug_messenger(context.instance.handle);
    context.surface = create_window_surface(context.instance.handle, window, instance);
    context.device.physical = create_physical_device(context.instance.handle, context.surface.handle, extensions);

    auto queue_family = get_queue_family(context.device.physical, context.surface.handle);
    if(!queue_family.has_value()) {
        return std::unexpected("queue_family not found");
    }
    auto& graphics_queue = queue_family.value().graphics;

    context.device.logical = create_logical_device(context.device.physical, queue_family.value(), extensions);
    context.queue = get_render_queue(context.device, queue_family.value().graphics, queue_family.value().presentation);
    Swapchain swapchain;
    VkSurfaceFormatKHR format {
        .format = VK_FORMAT_R8G8B8A8_UNORM,
        .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
    };
    create_swapchain(window->hwnd, context.device, context.surface, swapchain, format, VK_PRESENT_MODE_MAILBOX_KHR, nullptr);
    create_image_views(context.device, swapchain);
    context.swapchain = swapchain;

    Command command;
    command.pool = create_command_pool(context.device, graphics_queue);
    command.buffer = create_command_buffer(context.device, command.pool);
    context.command = command;

    context.command.render_passes.push_back(create_render_pass(context.device, swapchain.surface_format.format));
    context.pipeline = create_graphics_pipeline(context.device, swapchain.extent, context.command.render_passes[0]);
    create_framebuffers(context.device, context.swapchain, context.command.render_passes[0]);

    FrameData frame_data {
        .begin = create_semaphore(context.device),
        .in_flight = create_semaphore(context.device),
        .fence = create_fence(context.device)
    };
    context.frame_data = frame_data;

    return context;
}

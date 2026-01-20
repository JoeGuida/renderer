#include "api_vulkan.hpp"

#include <cassert>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <unordered_set>

#include <spdlog/spdlog.h>

#include "context.hpp"
#include "debug_messenger.hpp"
#include "device.hpp"
#include "extension.hpp"
#include "shader.hpp"
#include "swapchain.hpp"

bool is_gpu_usable(VkPhysicalDevice physical_device, VkSurfaceKHR surface, const std::vector<const char*> required_extensions) {
    if(!get_queue_family(physical_device, surface).has_value()) {
        return false;
    };

    if(!device_extensions_supported(physical_device, required_extensions)) {
        return false;
    }

    SwapchainSupportInfo swapchain_support = query_swapchain_support(physical_device, surface);
    return !swapchain_support.formats.empty() && !swapchain_support.present_modes.empty();
}

VkPhysicalDevice get_physical_device(VkInstance instance, VkSurfaceKHR surface, const std::vector<const char*>& device_extensions) {
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;

    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

    if(device_count == 0) {
        throw std::runtime_error("no physical devices found");
    }

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

    std::vector<int> scores;
    for(const auto& device : devices) {
        if(is_gpu_usable(device, surface, device_extensions)) {
            scores.push_back(score_device(device));
        }
        else {
            scores.push_back(0);
        }
    }

    int highest_score = 0;
    int highest_score_index = -1;
    for(int i = 0; i < scores.size(); i++) {
        if(scores[i] > highest_score) {
            highest_score = scores[i];
            highest_score_index = i;
        }
    }

    if(highest_score_index != -1) {
        physical_device = devices[highest_score_index];
    }

    if(physical_device == VK_NULL_HANDLE) {
        throw std::runtime_error("no suitable physical device was found");
    }

    return physical_device;
}

VkSurfaceFormatKHR choose_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats, VkFormat format, VkColorSpaceKHR color_space) {
    for(const auto& available_format : available_formats) {
        if(available_format.format == format && available_format.colorSpace == color_space) {
            return available_format;
        }
    }

    return available_formats[0];
}

VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes, VkPresentModeKHR mode) {
    for(const auto& available_mode : available_present_modes) {
        if(available_mode == mode) {
            return mode;
        }
    }

    return available_present_modes[0];
}

VkInstance create_instance(const std::vector<const char*> validation_layers, const std::vector<const char*> instance_extensions) {
    VkInstance instance;

    VkApplicationInfo application_info {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Renderer",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "EngineName",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0
    };

    VkInstanceCreateInfo create_info {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &application_info,
        .enabledLayerCount = static_cast<uint32_t>(validation_layers.size()),
        .ppEnabledLayerNames = validation_layers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(instance_extensions.size()),
        .ppEnabledExtensionNames = instance_extensions.data()
    };

    VkResult result = vkCreateInstance(&create_info, nullptr, &instance);
    if(result != VK_SUCCESS) {
        throw std::runtime_error("error creating vk instance");
    }

    return instance;
}

bool validation_layers_available(const std::vector<const char*>& validation_layers) {
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    for(const auto& layer : validation_layers) {
        bool layer_found = false;

        for(const auto& layer_properties : available_layers) {
            if(strcmp(layer, layer_properties.layerName) == 0) {
                layer_found = true;
                break;
            }
        }

        if(!layer_found) {
            return false;
        }
    }

    return true;
}

VkDevice create_logical_device(VkPhysicalDevice device, QueueFamily queue_family, const std::vector<const char*>& device_extensions) {
    float queue_priority = 1.0f;

    std::vector<uint32_t> unique_families { queue_family.graphics, queue_family.presentation };
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    queue_create_infos.reserve(unique_families.size());

    if(queue_family.graphics == queue_family.presentation) {
        VkDeviceQueueCreateInfo queue_create_info {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = queue_family.graphics,
            .queueCount = 1,
            .pQueuePriorities = &queue_priority
        };
        queue_create_infos.push_back(queue_create_info);
    }
    else {
        for(uint32_t queue_family : unique_families) {
            VkDeviceQueueCreateInfo queue_create_info {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = queue_family,
                .queueCount = 1,
                .pQueuePriorities = &queue_priority
            };
            queue_create_infos.push_back(queue_create_info);
        }
    }

    VkDevice logical_device;
    VkPhysicalDeviceFeatures device_features{};
    VkDeviceCreateInfo create_info {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size()),
        .pQueueCreateInfos = queue_create_infos.data(),
        .enabledExtensionCount = static_cast<uint32_t>(device_extensions.size()),
        .ppEnabledExtensionNames = device_extensions.data(),
        .pEnabledFeatures = &device_features
    };

    if(vkCreateDevice(device, &create_info, nullptr, &logical_device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device");
    }

    return logical_device;
}

VkSurfaceKHR create_window_surface(VkInstance vk_instance, HWND hwnd, HINSTANCE instance) {
    VkSurfaceKHR surface;

    VkWin32SurfaceCreateInfoKHR create_info {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .hinstance = instance,
        .hwnd = hwnd
    };

    if(vkCreateWin32SurfaceKHR(vk_instance, &create_info, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface");
    }

    return surface;
}

RenderQueue get_queues(VkDevice logical_device, uint32_t graphics_queuee_id, uint32_t presentation_queue_id) {
    VkQueue graphics_queue;
    vkGetDeviceQueue(logical_device, graphics_queuee_id, 0, &graphics_queue);

    VkQueue presentation_queue;
    vkGetDeviceQueue(logical_device, presentation_queue_id, 0, &presentation_queue);

    return RenderQueue {
        .graphics = graphics_queue,
        .presentation = presentation_queue
    };
}

std::vector<VkImageView> create_image_views(VkDevice device, const std::vector<VkImage>& swapchain_images, VkFormat image_format) {
    std::vector<VkImageView> swapchain_image_views;
    swapchain_image_views.resize(swapchain_images.size());

    for(size_t i = 0; i < swapchain_images.size(); i++) {
        VkImageViewCreateInfo create_info{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = swapchain_images[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = image_format,
            .components {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };

        if(vkCreateImageView(device, &create_info, nullptr, &swapchain_image_views[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views");
        }
    }

    return swapchain_image_views;
}

VkRenderPass create_render_pass(VkDevice device, VkFormat format) {
    VkAttachmentDescription color_attachment{
        .format = format,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };

    VkAttachmentReference color_attachment_reference {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    VkSubpassDescription subpass {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments = &color_attachment_reference
    };

    VkSubpassDependency dependency {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
    };

    VkRenderPass render_pass;
    VkPipelineLayout pipeline_layout;
    VkRenderPassCreateInfo render_pass_info {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = 1,
        .pAttachments = &color_attachment,
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 1,
        .pDependencies = &dependency
    };

    if(vkCreateRenderPass(device, &render_pass_info, nullptr, &render_pass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass");
    }

    return render_pass;
}

std::pair<VkPipelineLayout, VkPipeline> create_graphics_pipeline(VkDevice device, VkExtent2D extent, VkRenderPass render_pass) {
    auto vertex_shader_code = read_file(std::filesystem::current_path() / "shaders" / "vert.spv");
    auto fragment_shader_code = read_file(std::filesystem::current_path() / "shaders" / "frag.spv");

    VkShaderModule vertex_shader_module = create_shader_module(device, vertex_shader_code);
    VkShaderModule fragment_shader_module = create_shader_module(device, fragment_shader_code);

    VkPipelineShaderStageCreateInfo vertex_shader_stage_info{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_VERTEX_BIT,
        .module = vertex_shader_module,
        .pName = "main"
    };

    VkPipelineShaderStageCreateInfo fragment_shader_stage_info{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = fragment_shader_module,
        .pName = "main"
    };

    VkPipelineShaderStageCreateInfo shader_stages[] { vertex_shader_stage_info, fragment_shader_stage_info };

    std::vector<VkDynamicState> dynamic_states = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamic_state{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = static_cast<uint32_t>(dynamic_states.size()),
        .pDynamicStates = dynamic_states.data()
    };

    VkPipelineVertexInputStateCreateInfo vertex_input_info{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = 0,
        .pVertexBindingDescriptions = nullptr,
        .vertexAttributeDescriptionCount = 0,
        .pVertexAttributeDescriptions = nullptr
    };

    VkPipelineInputAssemblyStateCreateInfo input_assembly_info {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    };

    VkViewport viewport{
        .x = 0.0f,
        .y = 0.0f,
        .width = static_cast<float>(extent.width),
        .height = static_cast<float>(extent.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    };

    VkRect2D scissor{
        .offset = { 0, 0 },
        .extent = extent
    };

    VkPipelineViewportStateCreateInfo viewport_state {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .pViewports = &viewport,
        .scissorCount = 1,
        .pScissors = &scissor
    };

    VkPipelineRasterizationStateCreateInfo rasterization_state {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp = 0.0f,
        .depthBiasSlopeFactor = 0.0f,
        .lineWidth = 1.0f
    };

    VkPipelineMultisampleStateCreateInfo multisampling {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE,
        .minSampleShading = 1.0f,
        .pSampleMask = nullptr,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE
    };

    VkPipelineColorBlendAttachmentState color_blend_attachment {
        .blendEnable = VK_FALSE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    };

    VkPipelineColorBlendStateCreateInfo color_blend_state {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments = &color_blend_attachment,
        .blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f }
    };

    VkPipelineLayoutCreateInfo pipeline_layout_info {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 0,
        .pSetLayouts = nullptr,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr
    };

    VkPipelineLayout pipeline_layout;
    if(vkCreatePipelineLayout(device, &pipeline_layout_info, nullptr, &pipeline_layout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout");
    }

    VkGraphicsPipelineCreateInfo pipeline_info{
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = 2,
        .pStages = shader_stages,
        .pVertexInputState = &vertex_input_info,
        .pInputAssemblyState = &input_assembly_info,
        .pViewportState = &viewport_state,
        .pRasterizationState = &rasterization_state,
        .pMultisampleState = &multisampling,
        .pDepthStencilState = nullptr,
        .pColorBlendState = &color_blend_state,
        .pDynamicState = &dynamic_state,
        .layout = pipeline_layout,
        .renderPass = render_pass,
        .subpass = 0,
        .basePipelineHandle = VK_NULL_HANDLE,
        .basePipelineIndex = -1
    };

    VkPipeline graphics_pipeline;
    if(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &graphics_pipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline");
    }

    vkDestroyShaderModule(device, vertex_shader_module, nullptr);
    vkDestroyShaderModule(device, fragment_shader_module, nullptr);

    return { pipeline_layout, graphics_pipeline };
}

void create_framebuffers(VkDevice device, Swapchain& swapchain, VkRenderPass render_pass) {
    swapchain.framebuffers.resize(swapchain.image_views.size());

    for(size_t i = 0; i < swapchain.image_views.size(); i++) {
        VkImageView attachments[] = { swapchain.image_views[i] };

        VkFramebufferCreateInfo framebuffer_info {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = render_pass,
            .attachmentCount = 1,
            .pAttachments = attachments,
            .width = swapchain.extent.width,
            .height = swapchain.extent.height,
            .layers = 1
        };

        if(vkCreateFramebuffer(device, &framebuffer_info, nullptr, &swapchain.framebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer");
        }
    }
}

VkCommandPool create_command_pool(VkDevice device, uint32_t graphics_queue_family) {
    VkCommandPoolCreateInfo pool_info {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = graphics_queue_family
    };

    VkCommandPool command_pool;
    if(vkCreateCommandPool(device, &pool_info, nullptr, &command_pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool");
    }

    return command_pool;
}

VkCommandBuffer create_command_buffer(VkDevice device, VkCommandPool command_pool) {
    VkCommandBufferAllocateInfo alloc_info {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = command_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    VkCommandBuffer command_buffer;
    if(vkAllocateCommandBuffers(device, &alloc_info, &command_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers");
    }

    return command_buffer;
}

void record_command_buffer(Swapchain& swapchain, VkCommandBuffer command_buffer, VkRenderPass render_pass, VkFramebuffer framebuffer, VkPipeline graphics_pipeline) {
    VkCommandBufferBeginInfo begin_info {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = 0,
        .pInheritanceInfo = nullptr
    };

    if(vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer");
    }

    VkClearValue clear_color = {{{ 0.0f, 0.0f, 0.0f, 1.0f }}};
    VkRenderPassBeginInfo render_pass_begin_info {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = render_pass,
        .framebuffer = framebuffer,
        .renderArea = {
            .offset = { 0, 0 },
            .extent = swapchain.extent
        },
        .clearValueCount = 1,
        .pClearValues = &clear_color
    };

    vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);

    VkViewport viewport {
        .x = 0.0f,
        .y = 0.0f,
        .width = static_cast<float>(swapchain.extent.width),
        .height = static_cast<float>(swapchain.extent.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };

    vkCmdSetViewport(command_buffer, 0, 1, &viewport);

    VkRect2D scissor {
        .offset = { 0, 0 },
        .extent = swapchain.extent,
    };

    vkCmdSetScissor(command_buffer, 0, 1, &scissor);
    vkCmdDraw(command_buffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(command_buffer);
    if(vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer");
    }
};

std::pair<std::vector<VkSemaphore>, std::vector<VkFence>> create_sync_objects(VkDevice device) {
    VkSemaphore image_available_semaphore;
    VkSemaphore render_finished_semaphore;
    VkFence in_flight_fence;

    VkSemaphoreCreateInfo semaphore_info {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };

    VkFenceCreateInfo fence_info {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    if(vkCreateSemaphore(device, &semaphore_info, nullptr, &image_available_semaphore) != VK_SUCCESS ||
       vkCreateSemaphore(device, &semaphore_info, nullptr, &render_finished_semaphore) != VK_SUCCESS ||
       vkCreateFence(device, &fence_info, nullptr, &in_flight_fence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create semaphores");
    }

    return { { image_available_semaphore, render_finished_semaphore }, { in_flight_fence } };
}

void draw(VkContext context) {
    vkWaitForFences(context.device.logical, 1, &context.fences[0], VK_TRUE, UINT64_MAX);
    vkResetFences(context.device.logical, 1, &context.fences[0]);

    uint32_t image_index;
    vkAcquireNextImageKHR(context.device.logical, context.swapchain.handle, UINT64_MAX, context.semaphores[0], VK_NULL_HANDLE, &image_index);

    vkResetCommandBuffer(context.command_buffer, 0);
    record_command_buffer(context.swapchain, context.command_buffer, context.render_pass, context.swapchain.framebuffers[image_index], context.pipeline);

    VkSemaphore wait_semaphores[] = { context.semaphores[0] };
    VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkSemaphore signal_semaphores[] = { context.semaphores[1] };

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

    if(vkQueueSubmit(context.queue.graphics, 1, &submit_info, context.fences[0]) != VK_SUCCESS) {
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

    vkQueuePresentKHR(context.queue.presentation, &present_info);
}

std::expected<VkContext, std::string> init_renderer(PlatformWindow* window, HINSTANCE instance, const std::vector<const char*>& validation_layers, const std::vector<const char*>& instance_extensions, const std::vector<const char*>& device_extensions) {
    VkContext context = {};

    if(!validation_layers_available(validation_layers)) {
        return std::unexpected("validation layers are not available!");
    }

    if(!instance_extensions_supported(instance_extensions)) {
        return std::unexpected("requested instance extensions are not available");
    }

    // instance, physical device
    context.instance = create_instance(validation_layers, instance_extensions);
    context.debug_messenger = setup_debug_messenger(context.instance);
    context.surface = create_window_surface(context.instance, window->hwnd, instance);
    context.device.physical = get_physical_device(context.instance, context.surface, device_extensions);

    auto queue_family = get_queue_family(context.device.physical, context.surface);
    if(!queue_family.has_value()) {
        return std::unexpected("queue_family not found");
    }

    // logical device & swapchain
    context.device.logical = create_logical_device(context.device.physical, queue_family.value(), device_extensions);
    context.queue = get_queues(context.device.logical, queue_family.value().graphics, queue_family.value().presentation);
    auto swapchain = create_swapchain(window->hwnd, context.device, context.surface);
    SwapchainSupportInfo info = query_swapchain_support(context.device.physical, context.surface);
    context.swapchain.image_format = choose_surface_format(info.formats, VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR).format;
    context.swapchain.present_mode = choose_present_mode(info.present_modes, VK_PRESENT_MODE_MAILBOX_KHR);
    swapchain.image_views = create_image_views(context.device.logical, swapchain.images, swapchain.image_format);
    context.swapchain = swapchain;

    // graphics pipeline
    context.render_pass = create_render_pass(context.device.logical, swapchain.image_format);
    auto [pipeline_layout, graphics_pipeline] = create_graphics_pipeline(context.device.logical, swapchain.extent, context.render_pass);
    context.pipeline_layout = pipeline_layout;
    context.pipeline = graphics_pipeline;

    // commands, semaphore/fences
    create_framebuffers(context.device.logical, context.swapchain, context.render_pass);
    context.command_pool = create_command_pool(context.device.logical, queue_family.value().graphics);
    context.command_buffer = create_command_buffer(context.device.logical, context.command_pool);
    auto [semaphores, fences] = create_sync_objects(context.device.logical);
    context.semaphores = semaphores;
    context.fences = fences;

    return context;
}

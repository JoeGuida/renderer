#include "api_vulkan.hpp"

#include <filesystem>
#include <functional>
#include <unordered_set>

#include <spdlog/spdlog.h>

#include "shader.hpp"

bool is_gpu(VkPhysicalDevice device) {
    return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data) {
    if(severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
        spdlog::info("[debug] {}", callback_data->pMessage);
    }

    if(severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        spdlog::warn("[warn] {}", callback_data->pMessage);
    }

    if(severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        spdlog::error("[error] {}", callback_data->pMessage);
    }

    return VK_FALSE;
}

VkResult create_debug_utils_messenger_ext(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* create_info, const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* debug_messenger) {
    auto create = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if(create) {
        return create(instance, create_info, allocator, debug_messenger);
    }

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void destroy_debug_utils_messenger_ext(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger) {
    auto destroy = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if(destroy) {
        destroy(instance, debug_messenger, nullptr);
    }
}

VkDebugUtilsMessengerEXT vk_setup_debug_messenger(VkInstance instance) {
    VkDebugUtilsMessengerCreateInfoEXT create_info {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = debug_callback,
        .pUserData = nullptr
    };

    VkDebugUtilsMessengerEXT debug_messenger;
    if(create_debug_utils_messenger_ext(instance, &create_info, nullptr, &debug_messenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger");
    }

    return debug_messenger;
}

void log_supported_extensions(const std::unordered_set<std::string>& supported_instance_extensions, const std::unordered_set<std::string>& supported_device_extensions) {
    spdlog::info("Supported Instance Extensions:");
    spdlog::info("----------------------------------------------------------------------------------------------------");
    for(const auto& extension : supported_instance_extensions) {
        spdlog::info(extension);
    }

    spdlog::info("Supported Device Extensions:");
    spdlog::info("----------------------------------------------------------------------------------------------------");
    for(const auto& extension : supported_device_extensions) {
        spdlog::info(extension);
    }
}

bool validate_extensions(const std::unordered_set<std::string>& supported_instance_extensions, const std::unordered_set<std::string>& supported_device_extensions,
                         const std::vector<const char*>& instance_extensions, const std::vector<const char*>& device_extensions)
{
    for(const auto& extension : instance_extensions) {
        if(!supported_instance_extensions.contains(extension)) {
            spdlog::error("requested instance extension not found! :: {}", extension);
            return false;
        }
    }

    for(const auto& extension : device_extensions) {
        if(!supported_device_extensions.contains(extension)) {
            spdlog::error("requested device extension not found! :: {}", extension);
            return false;
        }
    }

    return true;
}

std::unordered_set<std::string> vk_get_supported_instance_extensions() {
    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> extensions(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

    std::unordered_set<std::string> supported_extensions;
    for(const auto& extension : extensions) {
        supported_extensions.insert(std::string(extension.extensionName));
    }

    return supported_extensions;
}

std::unordered_set<std::string> vk_get_supported_device_extensions(VkPhysicalDevice physical_device) {
    uint32_t device_extension_count = 0;
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &device_extension_count, nullptr);

    std::vector<VkExtensionProperties> device_extensions(device_extension_count);
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &device_extension_count, device_extensions.data());

    std::unordered_set<std::string> supported_extensions;
    for(const auto& extension : device_extensions) {
        supported_extensions.insert(std::string(extension.extensionName));
    }

    return supported_extensions;
}

VkInstance vk_create_instance(const std::vector<const char*> validation_layers, const std::vector<const char*> instance_extensions) {
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

VkPhysicalDevice vk_get_physical_device(VkInstance instance) {
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;

    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

    if(device_count == 0) {
        throw std::runtime_error("no physical devices found");
    }

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

    for(const auto& device : devices) {
        if(is_gpu(device)) {
            physical_device = device;
            break;
        }
    }

    if(physical_device == VK_NULL_HANDLE) {
        throw std::runtime_error("no suitable physical device was found");
    }

    return physical_device;
}

bool vk_enable_validation_layers(const std::vector<const char*>& validation_layers) {
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

std::pair<uint32_t, std::vector<VkDeviceQueueCreateInfo>> vk_find_graphics_queue_families(VkPhysicalDevice device) {
    std::optional<uint32_t> graphics_queue_family;
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

    int i = 0;
    for(const auto& queue_family : queue_families) {
        if(queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphics_queue_family = i;
            break;
        }

        i++;
    }

    if(!graphics_queue_family.has_value()) {
        throw std::runtime_error("could not find any valid graphics queue families");
    }

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    std::unordered_set<uint32_t> unique_queue_families { graphics_queue_family.value(), graphics_queue_family.value() };

    float queue_priority = 1.0f;
    for(uint32_t queue_family : unique_queue_families) {
        VkDeviceQueueCreateInfo queue_create_info {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = queue_family,
            .queueCount = 1,
            .pQueuePriorities = &queue_priority
        };
        queue_create_infos.push_back(queue_create_info);
    }

    return { graphics_queue_family.value(), queue_create_infos };
}

VkDevice vk_create_logical_device(VkPhysicalDevice physical_device, const std::vector<VkDeviceQueueCreateInfo>& queue_create_infos, const std::vector<const char*>& device_extensions) {
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

    if(vkCreateDevice(physical_device, &create_info, nullptr, &logical_device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device");
    }

    return logical_device;
}

VkSurfaceKHR vk_create_window_surface(VkInstance vk_instance, HWND hwnd, HINSTANCE instance) {
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

VkQueue vk_create_presentation_queue(VkDevice logical_device, uint32_t graphics_queue_family) {
    VkQueue presentation_queue;

    vkGetDeviceQueue(logical_device, graphics_queue_family, 0, &presentation_queue);

    return presentation_queue;
}

Swapchain vk_create_swapchain(HWND hwnd, VkPhysicalDevice physical_device, VkDevice logical_device, VkSurfaceKHR surface) {
    VkFormat surface_format = VK_FORMAT_B8G8R8A8_SRGB;
    VkColorSpaceKHR color_space = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    VkPresentModeKHR present_mode = VK_PRESENT_MODE_MAILBOX_KHR;

    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &capabilities);

    RECT client_rect;
    GetClientRect(hwnd, &client_rect);
    VkExtent2D extent = { 
        static_cast<uint32_t>(client_rect.right - client_rect.left),
        static_cast<uint32_t>(client_rect.bottom - client_rect.top)
    };

    uint32_t image_count = 2;

    VkSwapchainCreateInfoKHR create_info {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface,
        .minImageCount = image_count,
        .imageFormat = surface_format,
        .imageColorSpace = color_space,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = present_mode,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    };

    VkSwapchainKHR swapchain;
    if(vkCreateSwapchainKHR(logical_device, &create_info, nullptr, &swapchain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swapchain");
    }

    std::vector<VkImage> swapchain_images;
    vkGetSwapchainImagesKHR(logical_device, swapchain, &image_count, nullptr);
    swapchain_images.resize(image_count);
    vkGetSwapchainImagesKHR(logical_device, swapchain, &image_count, swapchain_images.data());

    return Swapchain {
        .swapchain = swapchain,
        .images = swapchain_images,
        .extent = extent,
        .image_format = surface_format
    };
}

std::vector<VkImageView> vk_create_image_views(VkDevice device, const std::vector<VkImage>& swapchain_images, VkFormat image_format) {
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

VkShaderModule vk_create_shader_module(VkDevice device, const std::vector<char>& code) {
    VkShaderModuleCreateInfo create_info{
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = code.size(),
        .pCode = reinterpret_cast<const uint32_t*>(code.data())
    };

    VkShaderModule shader_module;
    if(vkCreateShaderModule(device, &create_info, nullptr, &shader_module) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module");
    }

    return shader_module;
}

std::pair<VkRenderPass, VkRenderPassCreateInfo> vk_create_render_pass(VkDevice device, VkFormat format) {
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

    VkRenderPass render_pass;
    VkPipelineLayout pipeline_layout;
    VkRenderPassCreateInfo render_pass_info {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = 1,
        .pAttachments = &color_attachment,
        .subpassCount = 1,
        .pSubpasses = &subpass
    };

    if(vkCreateRenderPass(device, &render_pass_info, nullptr, &render_pass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass");
    }

    return { render_pass, render_pass_info };
}

std::pair<VkPipelineLayout, VkPipeline> vk_create_graphics_pipeline(VkDevice device, VkExtent2D extent, VkRenderPass render_pass) {
    auto vertex_shader_code = read_file(std::filesystem::current_path() / "shaders" / "vert.spv");
    auto fragment_shader_code = read_file(std::filesystem::current_path() / "shaders" / "frag.spv");

    VkShaderModule vertex_shader_module = vk_create_shader_module(device, vertex_shader_code);
    VkShaderModule fragment_shader_module = vk_create_shader_module(device, fragment_shader_code);

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

std::vector<VkFramebuffer> vk_create_framebuffers(VkDevice device, const std::vector<VkImageView>& swapchain_image_views, VkExtent2D swapchain_extent, VkRenderPass render_pass) {
    std::vector<VkFramebuffer> swapchain_framebuffers;
    swapchain_framebuffers.resize(swapchain_image_views.size());

    for(size_t i = 0; i < swapchain_image_views.size(); i++) {
        VkImageView attachments[] = { swapchain_image_views[i] };

        VkFramebufferCreateInfo framebuffer_info {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = render_pass,
            .attachmentCount = 1,
            .pAttachments = attachments,
            .width = swapchain_extent.width,
            .height = swapchain_extent.height,
            .layers = 1
        };

        if(vkCreateFramebuffer(device, &framebuffer_info, nullptr, &swapchain_framebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer");
        }
    }

    return swapchain_framebuffers;
}

VkCommandPool vk_create_command_pool(VkPhysicalDevice physical_device, VkDevice device) {
    uint32_t graphics_queue_family = vk_find_graphics_queue_families(physical_device).first;

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

VkCommandBuffer vk_create_command_buffer(VkDevice device, VkCommandPool command_pool) {
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

void vk_record_command_buffer(Swapchain& swapchain, VkCommandBuffer command_buffer, VkRenderPass render_pass, VkFramebuffer framebuffer, VkPipeline graphics_pipeline) {
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

std::pair<std::vector<VkSemaphore>, std::vector<VkFence>> vk_create_sync_objects(VkDevice device) {
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

void vk_draw(VkContext context) {
    vkWaitForFences(context.device, 1, &context.fences[0], VK_TRUE, UINT64_MAX);
    vkResetFences(context.device, 1, &context.fences[0]);

    uint32_t image_index;
    vkAcquireNextImageKHR(context.device, context.swapchain.swapchain, UINT64_MAX, context.semaphores[0], VK_NULL_HANDLE, &image_index);

    vkResetCommandBuffer(context.command_buffer, 0);
    vk_record_command_buffer(context.swapchain, context.command_buffer, context.render_pass, context.framebuffers[image_index], context.pipeline);

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

    if(vkQueueSubmit(context.presentation_queue, 1, &submit_info, context.fences[0]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer");
    }

    VkSubpassDependency dependency {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
    };

    //render_pass_info.dependencyCount = 1;
    //render_pass_info.pDependencies = &dependency;

    VkSwapchainKHR swapchains[] = { context.swapchain.swapchain };

    VkPresentInfoKHR present_info {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = signal_semaphores,
        .swapchainCount = 1,
        .pSwapchains = swapchains,
        .pImageIndices = &image_index,
        .pResults = nullptr
    };

    vkQueuePresentKHR(context.presentation_queue, &present_info);
}

void destroy_command_pool(VkDevice device, VkCommandPool command_pool) {
    vkDestroyCommandPool(device, command_pool, nullptr);
}

void destroy_device(VkDevice device) {
    vkDestroyDevice(device, nullptr);
}

void destroy_fence(VkDevice device, VkFence fence) {
    vkDestroyFence(device, fence, nullptr);
}

void destroy_framebuffer(VkDevice device, VkFramebuffer framebuffer) {
    vkDestroyFramebuffer(device, framebuffer, nullptr);
}

void destroy_image(VkDevice device, VkImage image) {
    vkDestroyImage(device, image, nullptr);
}

void destroy_image_view(VkDevice device, VkImageView image_view) {
    vkDestroyImageView(device, image_view, nullptr);
}

void destroy_instance(VkInstance instance) {
    vkDestroyInstance(instance, nullptr);
}

void destroy_pipeline(VkDevice device, VkPipeline pipeline) {
    vkDestroyPipeline(device, pipeline, nullptr);
}

void destroy_pipeline_layout(VkDevice device, VkPipelineLayout layout) {
    vkDestroyPipelineLayout(device, layout, nullptr);
}

void destroy_render_pass(VkDevice device, VkRenderPass render_pass) {
    vkDestroyRenderPass(device, render_pass, nullptr);
}

void destroy_semaphore(VkDevice device, VkSemaphore semaphore) {
    vkDestroySemaphore(device, semaphore, nullptr);
}

void destroy_surface(VkInstance instance, VkSurfaceKHR surface) {
    vkDestroySurfaceKHR(instance, surface, nullptr);
}

void destroy_swapchain(VkDevice device, VkSwapchainKHR swapchain) {
    vkDestroySwapchainKHR(device, swapchain, nullptr);
}

void vk_cleanup(VkContext context) {
    vkDeviceWaitIdle(context.device);

    for(auto& semaphore : context.semaphores) {
        destroy_semaphore(context.device, semaphore);
    }

    for(auto& fence : context.fences) {
        destroy_fence(context.device, fence);
    }

    destroy_command_pool(context.device, context.command_pool);

    for(auto& framebuffer : context.framebuffers) {
        destroy_framebuffer(context.device, framebuffer);
    }

    destroy_pipeline(context.device, context.pipeline);
    destroy_pipeline_layout(context.device, context.pipeline_layout);
    destroy_render_pass(context.device, context.render_pass);

    for(auto& image_view : context.swapchain.image_views) {
        destroy_image_view(context.device, image_view);
    }

    for(auto& image : context.swapchain.images) {
        //destroy_image(context.device, image);
    }

    destroy_swapchain(context.device, context.swapchain.swapchain);
    destroy_surface(context.instance, context.surface);
    destroy_device(context.device);
    destroy_debug_utils_messenger_ext(context.instance, context.debug_messenger);
    destroy_instance(context.instance);
}

std::expected<VkContext, std::string> vk_init(PlatformWindow* window, HINSTANCE instance, std::vector<const char*>& validation_layers, std::vector<const char*>& instance_extensions, std::vector<const char*>& device_extensions) {
    VkContext context;

    // instance & physical device
    context.instance = vk_create_instance(validation_layers, instance_extensions);
    context.debug_messenger = vk_setup_debug_messenger(context.instance);
    context.physical_device = vk_get_physical_device(context.instance);

    // validation_layers
    if(!vk_enable_validation_layers(validation_layers)) {
        return std::unexpected("validation layers requested, but not available!");
    }

    // extensions
    auto supported_instance_extensions = vk_get_supported_instance_extensions();
    auto supported_device_extensions = vk_get_supported_device_extensions(context.physical_device);
    if(!validate_extensions(supported_instance_extensions, supported_device_extensions, instance_extensions, device_extensions)) {
        return std::unexpected("requested extensions not available");
    }

    auto [graphics_queue_family, queue_create_infos] = vk_find_graphics_queue_families(context.physical_device);
    context.device = vk_create_logical_device(context.physical_device, queue_create_infos, device_extensions);
    context.surface = vk_create_window_surface(context.instance, window->hwnd, instance);
    context.presentation_queue = vk_create_presentation_queue(context.device, graphics_queue_family);
    auto swapchain = vk_create_swapchain(window->hwnd, context.physical_device, context.device, context.surface);
    swapchain.image_views = vk_create_image_views(context.device, swapchain.images, swapchain.image_format);
    context.swapchain = swapchain;

    auto [vk_render_pass, vk_render_pass_create_info] = vk_create_render_pass(context.device, swapchain.image_format);
    context.render_pass = vk_render_pass;

    auto [vk_pipeline_layout, vk_graphics_pipeline] = vk_create_graphics_pipeline(context.device, swapchain.extent, vk_render_pass);
    context.pipeline_layout = vk_pipeline_layout;
    context.pipeline = vk_graphics_pipeline;

    context.framebuffers = vk_create_framebuffers(context.device, swapchain.image_views, swapchain.extent, vk_render_pass);
    context.command_pool = vk_create_command_pool(context.physical_device, context.device);
    context.command_buffer = vk_create_command_buffer(context.device, context.command_pool);
    auto [vk_semaphores, vk_fences] = vk_create_sync_objects(context.device);
    context.semaphores = vk_semaphores;
    context.fences = vk_fences;

    return context;
}

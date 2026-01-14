#ifndef API_VULKAN_HPP
#define API_VULKAN_HPP

#if defined(WIN32) || defined(WIN64)

#define VK_USE_PLATFORM_WIN32_KHR

#include <platform/windows/win32_window.hpp>

#endif

#include <expected>
#include <unordered_set>
#include <vector>

#include <vulkan/vulkan.hpp>

struct Swapchain {
    VkSwapchainKHR swapchain;
    std::vector<VkImage> images;
    std::vector<VkImageView> image_views;
    VkExtent2D extent;
    VkFormat image_format;
};

struct VkContext {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    VkPhysicalDevice physical_device;
    VkDevice device;
    VkSurfaceKHR surface;
    VkQueue presentation_queue;
    Swapchain swapchain;
    VkRenderPass render_pass;
    VkPipeline pipeline;
    VkPipelineLayout pipeline_layout;
    VkCommandPool command_pool;
    VkCommandBuffer command_buffer;

    std::vector<VkFramebuffer> framebuffers;
    std::vector<VkSemaphore> semaphores;
    std::vector<VkFence> fences;
};

bool is_gpu(VkPhysicalDevice device);
VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data);
VkResult create_debug_utils_messenger_ext(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* create_info, const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* debug_messenger);
VkDebugUtilsMessengerEXT vk_setup_debug_messenger(VkInstance instance);
void log_supported_extensions(const std::unordered_set<std::string>& supported_instance_extensions, const std::unordered_set<std::string>& supported_device_extensions);
bool validate_extensions(const std::unordered_set<std::string>& supported_instance_extensions, const std::unordered_set<std::string>& supported_device_extensions,
                         const std::vector<const char*>& instance_extensions, const std::vector<const char*>& device_extensions);
std::unordered_set<std::string> vk_get_supported_instance_extensions();
std::unordered_set<std::string> vk_get_supported_device_extensions(VkPhysicalDevice physical_device);
VkInstance vk_create_instance(const std::vector<const char*> validation_layers, const std::vector<const char*> instance_extensions);
VkPhysicalDevice vk_get_physical_device(VkInstance instance);
bool vk_enable_validation_layers(const std::vector<const char*>& validation_layers);
std::pair<uint32_t, std::vector<VkDeviceQueueCreateInfo>> vk_find_graphics_queue_families(VkPhysicalDevice device);
VkDevice vk_create_logical_device(VkPhysicalDevice physical_device, const std::vector<VkDeviceQueueCreateInfo>& queue_create_infos, const std::vector<const char*>& device_extensions);
VkSurfaceKHR vk_create_window_surface(VkInstance vk_instance, HWND hwnd, HINSTANCE instance);
VkQueue vk_create_presentation_queue(VkDevice logical_device, uint32_t graphics_queue_family);
Swapchain vk_create_swapchain(HWND hwnd, VkPhysicalDevice physical_device, VkDevice logical_device, VkSurfaceKHR surface);
std::vector<VkImageView> vk_create_image_views(VkDevice device, const std::vector<VkImage>& swapchain_images, VkFormat image_format);
VkShaderModule vk_create_shader_module(VkDevice device, const std::vector<char>& code);
std::pair<VkRenderPass, VkRenderPassCreateInfo> vk_create_render_pass(VkDevice device, VkFormat format);
std::pair<VkPipelineLayout, VkPipeline> vk_create_graphics_pipeline(VkDevice device, VkExtent2D extent, VkRenderPass render_pass);
std::vector<VkFramebuffer> vk_create_framebuffers(VkDevice device, const std::vector<VkImageView>& swapchain_image_views, VkExtent2D swapchain_extent, VkRenderPass render_pass);
VkCommandPool vk_create_command_pool(VkPhysicalDevice physical_device, VkDevice device);
VkCommandBuffer vk_create_command_buffer(VkDevice device, VkCommandPool command_pool);
void vk_record_command_buffer(Swapchain& swapchain, VkCommandBuffer command_buffer, VkRenderPass render_pass, VkFramebuffer framebuffer, VkPipeline graphics_pipeline);
std::pair<std::vector<VkSemaphore>, std::vector<VkFence>> vk_create_sync_objects(VkDevice device);
void vk_draw(VkContext context);
void destroy_command_pool(VkDevice device, VkCommandPool command_pool);
void destroy_device(VkDevice device);
void destroy_fence(VkDevice device, VkFence fence);
void destroy_framebuffer(VkDevice device, VkFramebuffer framebuffer);
void destroy_image(VkDevice device, VkImage image);
void destroy_image_view(VkDevice device, VkImageView image_view);
void destroy_instance(VkInstance instance);
void destroy_pipeline(VkDevice device, VkPipeline pipeline);
void destroy_pipeline_layout(VkDevice device, VkPipelineLayout layout);
void destroy_render_pass(VkDevice device, VkRenderPass render_pass);
void destroy_semaphore(VkDevice device, VkSemaphore semaphore);
void destroy_surface(VkInstance instance, VkSurfaceKHR surface);
void destroy_swapchain(VkDevice device, VkSwapchainKHR swapchain);
void vk_cleanup(VkContext context);
std::expected<VkContext, std::string> vk_init(HWND hwnd, HINSTANCE instance, std::vector<const char*>& validation_layers, std::vector<const char*>& instance_extensions, std::vector<const char*>& device_extensions);

#endif

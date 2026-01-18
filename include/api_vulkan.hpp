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
bool validate_extensions(const std::unordered_set<std::string>& supported_instance_extensions, const std::unordered_set<std::string>& supported_device_extensions,
                         const std::vector<const char*>& instance_extensions, const std::vector<const char*>& device_extensions);
std::unordered_set<std::string> get_supported_instance_extensions();
std::unordered_set<std::string> get_supported_device_extensions(VkPhysicalDevice physical_device);
VkInstance create_instance(const std::vector<const char*> validation_layers, const std::vector<const char*> instance_extensions);
VkPhysicalDevice get_physical_device(VkInstance instance);
bool enable_validation_layers(const std::vector<const char*>& validation_layers);
std::pair<uint32_t, std::vector<VkDeviceQueueCreateInfo>> find_graphics_queue_families(VkPhysicalDevice device);
VkDevice create_logical_device(VkPhysicalDevice physical_device, const std::vector<VkDeviceQueueCreateInfo>& queue_create_infos, const std::vector<const char*>& device_extensions);
VkSurfaceKHR create_window_surface(VkInstance vk_instance, HWND hwnd, HINSTANCE instance);
VkQueue create_presentation_queue(VkDevice logical_device, uint32_t graphics_queue_family);
Swapchain create_swapchain(HWND hwnd, VkPhysicalDevice physical_device, VkDevice logical_device, VkSurfaceKHR surface);
std::vector<VkImageView> create_image_views(VkDevice device, const std::vector<VkImage>& swapchain_images, VkFormat image_format);
VkShaderModule create_shader_module(VkDevice device, const std::vector<char>& code);
std::pair<VkRenderPass, VkRenderPassCreateInfo> create_render_pass(VkDevice device, VkFormat format);
std::pair<VkPipelineLayout, VkPipeline> create_graphics_pipeline(VkDevice device, VkExtent2D extent, VkRenderPass render_pass);
std::vector<VkFramebuffer> create_framebuffers(VkDevice device, const std::vector<VkImageView>& swapchain_image_views, VkExtent2D swapchain_extent, VkRenderPass render_pass);
VkCommandPool create_command_pool(VkPhysicalDevice physical_device, VkDevice device);
VkCommandBuffer create_command_buffer(VkDevice device, VkCommandPool command_pool);
void record_command_buffer(Swapchain& swapchain, VkCommandBuffer command_buffer, VkRenderPass render_pass, VkFramebuffer framebuffer, VkPipeline graphics_pipeline);
std::pair<std::vector<VkSemaphore>, std::vector<VkFence>> create_sync_objects(VkDevice device);
void draw(VkContext context);

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
void cleanup(VkContext context);

std::expected<VkContext, std::string> init_renderer(PlatformWindow* window, HINSTANCE instance, std::vector<const char*>& validation_layers, std::vector<const char*>& instance_extensions, std::vector<const char*>& device_extensions);

#endif

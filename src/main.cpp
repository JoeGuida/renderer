#define VK_USE_PLATFORM_WIN32_KHR

#include <format>
#include <iostream>
#include <optional>
#include <set>
#include <stdexcept>
#include <utility>

#include <platform/platform.hpp>
#include <window/window.hpp>

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

#if defined(_WIN64) || defined(_WIN32)

#include <platform/windows/win32_window.hpp>
#include <vulkan/vulkan.hpp>

bool is_gpu(VkPhysicalDevice device) {
    return true;
}

VkInstance vk_create_instance(const std::vector<const char*> validation_layers) {
    VkInstance instance;

    VkApplicationInfo application_info {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "AppName",
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
        .enabledExtensionCount = 0,
        .ppEnabledExtensionNames = nullptr
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

    std::vector<VkPhysicalDevice> devices;
    devices.reserve(device_count);
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
    std::set<uint32_t> unique_queue_families { graphics_queue_family.value(), graphics_queue_family.value() };

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

VkDevice vk_create_logical_device(VkPhysicalDevice physical_device, const std::vector<VkDeviceQueueCreateInfo>& queue_create_infos) {
    VkDevice logical_device;
    VkPhysicalDeviceFeatures device_features{};
    VkDeviceCreateInfo create_info {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size()),
        .pQueueCreateInfos = queue_create_infos.data(),
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

std::pair<VkSwapchainKHR, std::vector<VkImage>> vk_create_swapchain(HWND hwnd, VkPhysicalDevice physical_device, VkDevice logical_device, VkSurfaceKHR surface) {
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

    return { swapchain, swapchain_images };
}

void vk_run() {

}

void vk_cleanup(VkInstance instance, VkDevice device, VkSurfaceKHR surface, VkSwapchainKHR swapchain) {
    vkDestroySwapchainKHR(device, swapchain, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
    vkDestroyDevice(device, nullptr);
}

int WinMain(HINSTANCE instance, HINSTANCE unused, LPSTR command_line, int show_window) {
    auto window_handle = initialize_window(instance, show_window, SCREEN_WIDTH, SCREEN_HEIGHT, L"window class", L"renderer");
    if(!window_handle.has_value()) {
        return EXIT_FAILURE;
    }

    Window window {
        .width = SCREEN_WIDTH,
        .height = SCREEN_HEIGHT,
        .handle = std::move(window_handle.value())
    };

    // Vulkan
    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    std::vector<const char*> validation_layers {
        "VK_LAYER_KHRONOS_validation"
    };

    vk_enable_validation_layers(validation_layers);
    auto vk_instance = vk_create_instance(validation_layers);
    auto vk_physical_device = vk_get_physical_device(vk_instance);
    auto [graphics_queue_family, queue_create_infos] = vk_find_graphics_queue_families(vk_physical_device);
    auto vk_logical_device = vk_create_logical_device(vk_physical_device, queue_create_infos);
    auto vk_surface = vk_create_window_surface(vk_instance, window.handle->hwnd, instance);
    auto presentation_queue = vk_create_presentation_queue(vk_logical_device, graphics_queue_family);
    auto [vk_swapchain, vk_swapchain_images] = vk_create_swapchain(window.handle->hwnd, vk_physical_device, vk_logical_device, vk_surface);

    vk_run();
    vk_cleanup(vk_instance, vk_logical_device, vk_surface, vk_swapchain);

    run_window(window.handle.get());

    return EXIT_SUCCESS;
}

#else

int main() {
    return EXIT_SUCCESS;
}

#endif

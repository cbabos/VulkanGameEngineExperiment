#include "Vulkan.h"

// @todo: Pass in metadata (app(name|version), engine(name|version)
void VulkanDriver::CreateVulkanInstance() {
  if (enableValidationLayers && !checkValidationLayerSupport()) {
    throw std::runtime_error("validation layers requested, but not available!");
  }

  VkApplicationInfo appInfo{};
  appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName   = "Vulkan App";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName        = "No Engine";
  appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion         = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
  if (enableValidationLayers) {
    createInfo.enabledLayerCount =
      static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    PopulateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.pNext             = nullptr;
  }

  auto requiredExtensions            = getRequiredExtensions();
  createInfo.enabledExtensionCount   = (uint32_t) requiredExtensions.size();
  createInfo.ppEnabledExtensionNames = requiredExtensions.data();
  createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

  VkResult vkResult = vkCreateInstance(&createInfo, nullptr, &instance);
  if (vkResult != VK_SUCCESS) {
    char *error;
    switch (vkResult) {
      case VK_ERROR_OUT_OF_HOST_MEMORY:
        throw std::runtime_error("Out of memory");
        break;
      case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        throw std::runtime_error("Out of device memory");
        break;
      case VK_ERROR_INITIALIZATION_FAILED:
        throw std::runtime_error("Initialization failed");
        break;
      case VK_ERROR_LAYER_NOT_PRESENT:
        throw std::runtime_error("Layer not present");
        break;
      case VK_ERROR_EXTENSION_NOT_PRESENT:
        throw std::runtime_error("Extension not present");
        break;
      case VK_ERROR_INCOMPATIBLE_DRIVER:
        throw std::runtime_error("Incompatible driver");
        break;
      default: throw std::runtime_error("Unknown error"); break;
    }
  }

  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  std::vector<VkExtensionProperties> extensions(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                         extensions.data());
}

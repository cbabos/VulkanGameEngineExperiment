#include "Vulkan.h"
#include <set>

void VulkanDriver::PickPhysicalDevice() {
  uint32_t deviceCount = 0;
  VkResult result = vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

  if (result != VK_SUCCESS) {
    switch (result) {
      case VK_INCOMPLETE:
        throw std::runtime_error("PickPhysicalDevice::VK_INCOMPLETE");
        break;
      case VK_ERROR_OUT_OF_HOST_MEMORY:
        throw std::runtime_error(
          "PickPhysicalDevice::VK_ERROR_OUT_OF_HOST_MEMORY");
        break;
      case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        throw std::runtime_error(
          "PickPhysicalDevice::VK_ERROR_OUT_OF_DEVICE_MEMORY");
        break;
      case VK_ERROR_INITIALIZATION_FAILED:
        throw std::runtime_error(
          "PickPhysicalDevice::VK_ERROR_INITIALIZATION_FAILED");
        break;
      default: throw std::runtime_error("PickPhysicalDevice::Unknown"); break;
    }
  }

  if (deviceCount == 0) {
    throw std::runtime_error("No Vulkan-supported GPUs were found");
  }

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

  for (const auto &device : devices) {
    if (isDeviceSuitable(device)) {
      physicalDevice = device;
      break;
    }
  }

  if (physicalDevice == VK_NULL_HANDLE) {
    throw std::runtime_error("Failed to find suitable GPU");
  }
}

bool VulkanDriver::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());

  std::set<std::string> requiredExtensions(deviceExtensions.begin(),
                                           deviceExtensions.end());
  
  for (const auto &extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}

bool VulkanDriver::isDeviceSuitable(VkPhysicalDevice device) {
  QueueFamilyIndices indices = FindQueueFamilies(device);

  bool extensionsSupported = CheckDeviceExtensionSupport(device);
  bool swapChainAdequate   = false;
  if (extensionsSupported) {
    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
    swapChainAdequate = !swapChainSupport.formats.empty() &&
                        !swapChainSupport.presentModes.empty();
  }

  return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

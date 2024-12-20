#include "Vulkan.h"
#include <iostream>

std::vector<const char *> getRequiredExtensions() {
  uint32_t     glfwExtensionCount = 0;
  const char **glfwExtensions;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char *> requiredExtensions;

  for (uint32_t i = 0; i < glfwExtensionCount; i++) {
    requiredExtensions.emplace_back(glfwExtensions[i]);
  }

  requiredExtensions.emplace_back(
    VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
  requiredExtensions.push_back(
    VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

  if (enableValidationLayers) {
    requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  for (auto ext : requiredExtensions) {
	  std::cout << "Extension: " << ext << std::endl;
  }

  return requiredExtensions;
}

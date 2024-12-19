#include "Vulkan.h"

void VulkanDriver::CreateVulkanSurface() {
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create VkSurface");
	}
}

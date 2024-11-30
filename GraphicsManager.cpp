#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan_core.h>
using namespace std;

#include "GraphicsManager.h"

bool GraphicsManager::isInitialized() {
	return isManagerUp;
}

GraphicsManager::GraphicsManager(int width, int height, const char* title) {
	if (!glfwInit()) {
		cerr << "Failed to initialize GLFW" << endl;
		window = nullptr;
		return;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // No OpenGL please 

	cout << "Creating new window [" << width << "x" << height << " | " 
		<< title << endl;

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window) {
		cerr << "Failed to create window" << endl;
		glfwTerminate();
		return;
	}

	try {
		InitVulkan();
	} catch (const exception& e) {
		cerr << e.what() << endl;
		return;
	}

	glfwMakeContextCurrent(window);
	isManagerUp = GLFW_TRUE;
}

void GraphicsManager::InitVulkan() {
	CreateVulkanInstance();
	CreateVulkanSurface();
}

void GraphicsManager::CreateVulkanInstance() {
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan App";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	u_int32_t extensionCount = 3;
	const char* extensions [3] = { 
		"VK_KHR_surface", 
		"VK_EXT_metal_surface", 
		"VK_KHR_portability_enumeration" 
	}; 

	createInfo.enabledExtensionCount = extensionCount;
	createInfo.ppEnabledExtensionNames = extensions;
	createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

	VkResult vkResult = vkCreateInstance(&createInfo, nullptr, &instance);
	if (vkResult != VK_SUCCESS) {
		char* error;
		switch (vkResult) {
			case VK_ERROR_OUT_OF_HOST_MEMORY:
				throw runtime_error("Out of memory");
				break;
			case VK_ERROR_OUT_OF_DEVICE_MEMORY: 
				throw runtime_error("Out of device memory");
				break;
			case VK_ERROR_INITIALIZATION_FAILED: 
				throw runtime_error("Initialization failed");
				break;
			case VK_ERROR_LAYER_NOT_PRESENT: 
				throw runtime_error("Layer not present");
				break;
			case VK_ERROR_EXTENSION_NOT_PRESENT: 
				throw runtime_error("Extension not present");
				break;
			case VK_ERROR_INCOMPATIBLE_DRIVER: 
				throw runtime_error("Incompatible driver");
				break;
			default: 
				throw runtime_error("Unknown error");
				break;
		}
	}
}

void GraphicsManager::CreateVulkanSurface() {
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw runtime_error("Failed to create VkSurface");
	}
}

void GraphicsManager::PickPhysicalDevice() {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount) { 
		throw runtime_error("No Vulkan-supported GPUs were found");
	}

	vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
	
	for (const auto& device : devices) {
		if (isDeviceSuitable(device)) {
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE) {
		throw runtime_error("Failed to find suitable GPU");
	}
}

bool GraphicsManager::isDeviceSuitable(VkPhysicalDevice device) {
	// Maybe need to add GPU checks if needed
	return true;
}

void GraphicsManager::CreateLogicalDevice() {
	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	VkPhysicalDeviceFeatures deviceFeatures{};

	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = 0;

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
		throw runtime_error("Failed to create logical device");
	}

	vkGetDeviceQueue(device, 0, 0, &graphicsQueue);
}

void GraphicsManager::CreateSwapChain() {
	// TODO: what's coming here?
}

void GraphicsManager::CreateImageViews() {
	// TODO: what's coming here?
}

void GraphicsManager::DestroyVulkan() {
	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(device, swapChain, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();
}

GLFWwindow* GraphicsManager::getWindow() const {
	return window;
}

void GraphicsManager::destroyWindow() {
	cout << "Destroying main window..." << endl;
	if (window) { 
		glfwDestroyWindow(window);
		window = nullptr;
	}
	glfwTerminate();
}

bool GraphicsManager::shouldClose() {
	return glfwWindowShouldClose(window);
}

void GraphicsManager::update() {
	glfwSwapBuffers(window);
}

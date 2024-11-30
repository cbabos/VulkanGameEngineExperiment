#include <iostream>
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

	if (!CreateVulkanInstance()) {
		glfwTerminate();
		return;
	}

	cout << "Creating new window [" << width << "x" << height << " | " 
		 << title << endl;

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window) {
		cerr << "Failed to create window" << endl;
		DestroyVulkanInstance();
		glfwTerminate();
		return;
	}

    glfwMakeContextCurrent(window);
	isManagerUp = GLFW_TRUE;
}

bool GraphicsManager::CreateVulkanInstance() {
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

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

	VkResult vkResult = vkCreateInstance(&createInfo, nullptr, &instance);
    if (vkResult != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan instance!" << std::endl;

		switch (vkResult) {
			case VK_ERROR_OUT_OF_HOST_MEMORY:
				cerr << "Out of memory" << endl;
				break;
			case VK_ERROR_OUT_OF_DEVICE_MEMORY: 
				cerr << "Out of device memory" << endl;
				break;
			case VK_ERROR_INITIALIZATION_FAILED: 
				cerr << "Initialization failed" << endl;
				break;
			case VK_ERROR_LAYER_NOT_PRESENT: 
				cerr << "Layer not present" << endl;
				break;
			case VK_ERROR_EXTENSION_NOT_PRESENT: 
				cerr << "Extension not present" << endl;
				break;
			case VK_ERROR_INCOMPATIBLE_DRIVER: 
				cerr << "Incompatible driver" << endl;
				break;
			default: 
				cerr << "Unknown error" << endl;
				break;
		}

		return false;
    }
	return true;
}

void GraphicsManager::DestroyVulkanInstance() {
	vkDestroyInstance(instance, nullptr);
}

GLFWwindow* GraphicsManager::getWindow() const {
	return window;
}

void GraphicsManager::destroyWindow() {
	DestroyVulkanInstance();

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

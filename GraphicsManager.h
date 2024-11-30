#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
using namespace std;

class GraphicsManager {
	protected:
	GLFWwindow* window; 
	VkInstance instance;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkSwapchainKHR swapChain;
	vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	vector<VkImageView> swapChainImageViews;
	bool isManagerUp = false;
	
	void InitVulkan();
	void CreateVulkanInstance();
	void CreateVulkanSurface();
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	void CreateSwapChain();
	void CreateImageViews();
	void DestroyVulkan();
	bool isDeviceSuitable(VkPhysicalDevice device);

	public: 
	GraphicsManager(int width, int height, const char* title);
	GLFWwindow* getWindow() const;
	bool shouldClose();
	bool isInitialized();
	void update();
	void destroyWindow();
}; 

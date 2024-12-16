#ifndef VULKANDRIVER_H
#define VULKANDRIVER_H

#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../IGraphicsDriver.h"

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {

		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VulkanDriver : public IGraphicsDriver {
	public: 
		VulkanDriver();
		~VulkanDriver();

		void Setup(GLFWwindow* window) override;
		void Destruct() override;
		void RenderFrame() override;

	private: 
		GLFWwindow* window;

		VkInstance instance;
		VkSurfaceKHR surface;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkDebugUtilsMessengerEXT debugMessenger;

		VkSwapchainKHR swapChain;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		VkPipeline graphicsPipeline;
		VkRenderPass renderPass;
		VkPipelineLayout pipelineLayout;
		VkCommandPool commandPool;
		VkCommandBuffer commandBuffer;
		
		std::vector<VkImageView> swapChainImageViews;
		std::vector<VkImage> swapChainImages;
		std::vector<VkFramebuffer> swapChainFramebuffers;


		void InitVulkan();
		void CreateVulkanInstance();
		void CreateVulkanSurface();
		void PickPhysicalDevice();
		void CreateLogicalDevice();
		void CreateSwapChain();
		void CreateImageViews();
		void CreateGraphicsPipeline();
		void DestroyVulkan();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void SetupDebugMessenger();
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
		void CreateRenderPass(); 
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		bool isDeviceSuitable(VkPhysicalDevice device);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		void CreateFrameBuffers();
		void CreateCommandPool();
		void CreateCommandBuffer();
		void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		void DrawFrame(); 
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device); 
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		VkShaderModule CreateShaderModule(const std::vector<char>& code);
};

#endif

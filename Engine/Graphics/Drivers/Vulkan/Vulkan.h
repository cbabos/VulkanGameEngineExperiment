#ifndef VULKANDRIVER_H
#define VULKANDRIVER_H
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include "../IGraphicsDriver.h"
#include "Vertex.h"

#include <GLFW/glfw3.h>
#include <vector>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char *> validationLayers = {
  "VK_LAYER_KHRONOS_validation"};
const std::vector<const char *> deviceExtensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME,
  // @todo Need to figure out a way to check if needed
  "VK_KHR_portability_subset"};

const int MAX_FRAMES_IN_FLIGHT = 2;

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete();
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   presentModes;
};

struct UniformBufferObject { 
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

bool                      checkValidationLayerSupport();
std::vector<const char *> getRequiredExtensions();

class VulkanDriver : public IGraphicsDriver {
  public:
    VulkanDriver();
    ~VulkanDriver();

    void Setup(GLFWwindow *window) override;
    void Destruct() override;
    void RenderFrame() override;
    void WindowIsResized() override;

  private:
    GLFWwindow *window;

    VkInstance               instance;
    VkSurfaceKHR             surface;
    VkPhysicalDevice         physicalDevice = VK_NULL_HANDLE;
    VkDevice                 device;
    VkQueue                  graphicsQueue;
    VkQueue                  presentQueue;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkSwapchainKHR   swapChain;
    VkFormat         swapChainImageFormat;
    VkExtent2D       swapChainExtent;
    VkPipeline       graphicsPipeline;
    VkRenderPass     renderPass;
	VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkCommandPool    commandPool;
    VkBuffer         vertexBuffer;
    VkDeviceMemory   vertexBufferMemory;

    VkBuffer       indexBuffer;
    VkDeviceMemory indexBufferMemory;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence>     inFlightFences;

    bool framebufferResized = false;

    std::vector<VkImageView>   swapChainImageViews;
    std::vector<VkImage>       swapChainImages;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    uint32_t currentFrame = 0;

    void InitVulkan();
    void CreateVulkanInstance();
    void CreateVulkanSurface();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSwapChain();
    void RecreateSwapChain();
    void CleanupSwapChain();
    void CreateImageViews();
	void CreateDescriptorSetLayout();
    void CreateGraphicsPipeline();
    void CreateVertexBuffer();
	void CreateUniformBuffers();
	void CreateIndexBuffer();
	void CreateDescriptorPool();
	void CreateDescriptorSets();
    void DestroyVulkan();
    void PopulateDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    void               SetupDebugMessenger();
    void               DestroyDebugUtilsMessengerEXT(VkInstance               instance,
                                                     VkDebugUtilsMessengerEXT debugMessenger,
                                                     const VkAllocationCallbacks *pAllocator);
    void               CreateRenderPass();
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    bool               isDeviceSuitable(VkPhysicalDevice device);
    bool               CheckDeviceExtensionSupport(VkPhysicalDevice device);
    void               CreateFrameBuffers();
    void               CreateCommandPool();
    void               CreateCommandBuffers();
    void               CreateSyncObjects();
    void               RecordCommandBuffer(VkCommandBuffer commandBuffer,
                                           uint32_t        imageIndex);
    void               DrawFrame();
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR      ChooseSwapSurfaceFormat(
           const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(
      const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
    VkShaderModule CreateShaderModule(const std::vector<char> &code);
    uint32_t       FindMemoryType(uint32_t              typeFilter,
                                  VkMemoryPropertyFlags properties);
    void           CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                                VkMemoryPropertyFlags properties, VkBuffer &buffer,
                                VkDeviceMemory &bufferMemory);
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void UpdateUniformBuffer(uint32_t currentImage);
};

#endif

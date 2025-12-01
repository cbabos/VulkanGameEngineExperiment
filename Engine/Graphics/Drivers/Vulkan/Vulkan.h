#ifndef VULKANDRIVER_H
#define VULKANDRIVER_H
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include "../IGraphicsDriver.h"
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "Vertex.h"
#include "Mesh.h"
#include "Texture.h"
#include "RenderObject.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>
#include <unordered_map>

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

// Removed hardcoded paths - models/textures are loaded via API

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   presentModes;
};

struct UniformBufferObject {
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

bool                      checkValidationLayerSupport();
std::vector<const char *> getRequiredExtensions();

// Internal mesh data structure for Vulkan resources
struct VulkanMesh {
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    uint32_t indexCount;
};

// Internal texture data structure
struct VulkanTexture {
    VkImage image;
    VkDeviceMemory imageMemory;
    VkImageView imageView;
    VkSampler sampler;
};

class VulkanDriver : public IGraphicsDriver {
  public:
    VulkanDriver();
    ~VulkanDriver();

    void Setup(GLFWwindow *window) override;
    void Destruct() override;
    void RenderFrame() override;
    void WindowIsResized() override;
    
    // IGraphicsDriver API
    std::shared_ptr<Mesh> LoadMesh(const std::string& modelPath) override;
    std::shared_ptr<Texture> LoadTexture(const std::string& texturePath) override;
    std::shared_ptr<Mesh> CreateMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) override;
    std::shared_ptr<Texture> CreateTexture(uint32_t width, uint32_t height, const void* pixelData) override;
    void SubmitRenderObject(const RenderObject& renderObject) override;
    void ClearRenderQueue() override;
    void SetViewMatrix(const glm::mat4& view) override;
    void SetProjectionMatrix(const glm::mat4& projection) override;

  private:
    GLFWwindow *window;

    VkInstance               instance;
    VkSurfaceKHR             surface;
    VkPhysicalDevice         physicalDevice = VK_NULL_HANDLE;
    VkDevice                 device;
    VkQueue                  graphicsQueue;
    VkQueue                  presentQueue;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkSwapchainKHR        swapChain;
    VkFormat              swapChainImageFormat;
    VkExtent2D            swapChainExtent;
    VkPipeline            graphicsPipeline;
    VkRenderPass          renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout      pipelineLayout;
    VkCommandPool         commandPool;
    VkSampler             defaultTextureSampler;  // Shared sampler for all textures
    
    // Default white texture for descriptor set initialization
    VkImage        defaultTextureImage;
    VkDeviceMemory defaultTextureImageMemory;
    VkImageView    defaultTextureImageView;

    VkImage        depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView    depthImageView;

    VkDescriptorPool             descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    std::vector<VkBuffer>       uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void *>         uniformBuffersMapped;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence>     inFlightFences;

    bool framebufferResized = false;

    std::vector<VkImageView>   swapChainImageViews;
    std::vector<VkImage>       swapChainImages;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    uint32_t currentFrame = 0;
    
    // Resource management
    std::unordered_map<std::shared_ptr<Mesh>, VulkanMesh> meshResources;
    std::unordered_map<std::shared_ptr<Texture>, VulkanTexture> textureResources;
    std::unordered_map<std::shared_ptr<Texture>, std::vector<VkDescriptorSet>> textureDescriptorSets;  // Per-texture descriptor sets
    
    // Render queue
    std::vector<RenderObject> renderQueue;
    
    // Camera matrices
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);

    void InitVulkan();
    void CreateVulkanInstance();
    
    // Resource creation helpers
    VulkanMesh CreateVulkanMesh(const Mesh& mesh);
    void DestroyVulkanMesh(VulkanMesh& vulkanMesh);
    VulkanTexture CreateVulkanTexture(const std::string& texturePath);
    void DestroyVulkanTexture(VulkanTexture& vulkanTexture);
    void CreateVulkanSurface();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSwapChain();
    void RecreateSwapChain();
    void CleanupSwapChain();
    void CreateImageViews();
    void CreateDescriptorSetLayout();
    void CreateGraphicsPipeline();
    void CreateUniformBuffers();
    void CreateDescriptorPool();
    void CreateDescriptorSets();
    void CreateDepthResources();
    void CreateDefaultTextureSampler();
    void CreateDefaultTexture();
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
    void CreateImage(uint32_t width, uint32_t height, VkFormat format,
                     VkImageTiling tiling, VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties, VkImage &image,
                     VkDeviceMemory &imageMemory);
    VkCommandBuffer BeginSingleTimeCommands();
    void            EndSingleTimeCommands(VkCommandBuffer commandBuffer);
    void            TransitionImageLayout(VkImage image, VkFormat format,
                                          VkImageLayout oldLayout,
                                          VkImageLayout newLayout);
    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
                           uint32_t height);
    VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    VkFormat    FindSupportedFormat(const std::vector<VkFormat> &candidates,
                                    VkImageTiling                tiling,
                                    VkFormatFeatureFlags         features);
	VkFormat FindDepthFormat();
	bool HasStencilComponent(VkFormat format);
};

#endif

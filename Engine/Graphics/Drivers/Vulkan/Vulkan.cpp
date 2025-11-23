#include "Vulkan.h"

#include <cstddef>
#include <iostream>
#include <vulkan/vulkan_core.h>

VulkanDriver::VulkanDriver() {}

VulkanDriver::~VulkanDriver() {}

void VulkanDriver::RenderFrame() {
  DrawFrame();
}

void VulkanDriver::Setup(GLFWwindow *engineWindow) {
  window = engineWindow;
  InitVulkan();
}

void VulkanDriver::Destruct() {
  DestroyVulkan();
}

void VulkanDriver::InitVulkan() {
  CreateVulkanInstance();
  SetupDebugMessenger();
  CreateVulkanSurface();
  PickPhysicalDevice();
  CreateLogicalDevice();
  CreateSwapChain();
  CreateImageViews();
  CreateRenderPass();
  CreateDescriptorSetLayout();
  CreateGraphicsPipeline();
  CreateCommandPool();
  CreateDepthResources();
  CreateFrameBuffers();
  CreateDefaultTextureSampler();
  CreateUniformBuffers();
  CreateDescriptorPool();
  CreateDescriptorSets();
  CreateCommandBuffers();
  CreateSyncObjects();
}

void VulkanDriver::WindowIsResized() {
	framebufferResized = true;
}

void VulkanDriver::DestroyVulkan() {
  vkDeviceWaitIdle(device);

  // Clean up mesh resources
  for (auto& [mesh, vulkanMesh] : meshResources) {
    DestroyVulkanMesh(vulkanMesh);
  }
  meshResources.clear();

  // Clean up texture resources
  for (auto& [texture, vulkanTexture] : textureResources) {
    DestroyVulkanTexture(vulkanTexture);
  }
  textureResources.clear();

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
    vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
    vkDestroyFence(device, inFlightFences[i], nullptr);

	vkDestroyBuffer(device, uniformBuffers[i], nullptr);
	vkFreeMemory(device, uniformBuffersMemory[i], nullptr); 
  }

  vkDestroyCommandPool(device, commandPool, nullptr);

  CleanupSwapChain();
  vkDestroyDescriptorPool(device, descriptorPool, nullptr);
  vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

  vkDestroySampler(device, defaultTextureSampler, nullptr);

  vkDestroyPipeline(device, graphicsPipeline, nullptr);
  vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
  vkDestroyRenderPass(device, renderPass, nullptr);

  vkDestroyDevice(device, nullptr);
  vkDestroySurfaceKHR(instance, surface, nullptr);

  if (enableValidationLayers) {
    DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
  }

  vkDestroyInstance(instance, nullptr);
}

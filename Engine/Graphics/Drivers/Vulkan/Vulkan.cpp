#include "Vulkan.h"
#include <cstddef>

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
  CreateGraphicsPipeline();
  CreateFrameBuffers();
  CreateCommandPool();
  CreateCommandBuffers();
  CreateSyncObjects();
}

void VulkanDriver::DestroyVulkan() {
  vkDeviceWaitIdle(device);
  
  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
	  vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
	  vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
	  vkDestroyFence(device, inFlightFences[i], nullptr);
  }

  vkDestroyCommandPool(device, commandPool, nullptr);

  for (auto framebuffer : swapChainFramebuffers) {
    vkDestroyFramebuffer(device, framebuffer, nullptr);
  }

  vkDestroyPipeline(device, graphicsPipeline, nullptr);
  vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
  vkDestroyRenderPass(device, renderPass, nullptr);

  for (auto imageView : swapChainImageViews) {
    vkDestroyImageView(device, imageView, nullptr);
  }

  vkDestroySwapchainKHR(device, swapChain, nullptr);
  vkDestroyDevice(device, nullptr);
  vkDestroySurfaceKHR(instance, surface, nullptr);

  if (enableValidationLayers) {
    DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
  }

  vkDestroyInstance(instance, nullptr);
}

#include "Vulkan.h"

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
  CreateCommandBuffer();
  CreateSyncObjects();
}

void VulkanDriver::DestroyVulkan() {
  vkDeviceWaitIdle(device);

  vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
  vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
  vkDestroyFence(device, inFlightFence, nullptr);

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

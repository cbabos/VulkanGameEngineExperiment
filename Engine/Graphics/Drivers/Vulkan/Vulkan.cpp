#include "Vulkan.h"

#include <cstddef>
#include <iostream>

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
  CreateVertexBuffer();
  CreateCommandBuffers();
  CreateSyncObjects();
}

void VulkanDriver::WindowIsResized() {
	framebufferResized = true;
}

void VulkanDriver::DestroyVulkan() {
  vkDeviceWaitIdle(device);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
    vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
    vkDestroyFence(device, inFlightFences[i], nullptr);
  }

  vkDestroyCommandPool(device, commandPool, nullptr);

  CleanupSwapChain();
  vkDestroyBuffer(device, vertexBuffer, nullptr);
  vkFreeMemory(device, vertexBufferMemory, nullptr);

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

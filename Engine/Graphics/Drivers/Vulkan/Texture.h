#ifndef TEXTURE_H
#define TEXTURE_H

#include <vulkan/vulkan_core.h>
#include <string>

// Forward declaration
class VulkanDriver;

// Texture represents a loaded texture image
class Texture {
public:
    Texture() : image(VK_NULL_HANDLE), imageMemory(VK_NULL_HANDLE), 
                imageView(VK_NULL_HANDLE), sampler(VK_NULL_HANDLE) {}
    
    VkImage image;
    VkDeviceMemory imageMemory;
    VkImageView imageView;
    VkSampler sampler;
    
    // Only VulkanDriver should create textures
    friend class VulkanDriver;
};

#endif // TEXTURE_H


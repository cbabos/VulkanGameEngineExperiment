#include "Vulkan.h"
#include "RenderObject.h"
#include "../../../../Utils/FileUtils.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <unordered_map>
#include <iostream>
#include <cstring>

std::shared_ptr<Mesh> VulkanDriver::LoadMesh(const std::string& modelPath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    std::unordered_map<Vertex, uint32_t> uniqueVertices{};
    
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str())) {
        throw std::runtime_error("Failed to load model: " + warn + err);
    }

    for (const auto& shape: shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };
            
            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = {1.0f, 1.0f, 1.0f};

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }
            
            indices.push_back(uniqueVertices[vertex]);
        }
    }

    std::cout << "Loaded mesh from " << modelPath << ": " << vertices.size() 
              << " vertices, " << indices.size() << " indices" << std::endl;

    auto mesh = std::make_shared<Mesh>(vertices, indices);
    
    // Create Vulkan resources for this mesh
    meshResources[mesh] = CreateVulkanMesh(*mesh);
    
    return mesh;
}

std::shared_ptr<Texture> VulkanDriver::LoadTexture(const std::string& texturePath) {
    auto texture = std::make_shared<Texture>();
    
    // Create Vulkan resources for this texture
    textureResources[texture] = CreateVulkanTexture(texturePath);
    
    // Update texture object with Vulkan handles
    texture->image = textureResources[texture].image;
    texture->imageMemory = textureResources[texture].imageMemory;
    texture->imageView = textureResources[texture].imageView;
    texture->sampler = defaultTextureSampler;  // Use shared sampler
    
    std::cout << "Loaded texture from " << texturePath << std::endl;
    
    return texture;
}

void VulkanDriver::SubmitRenderObject(const RenderObject& renderObject) {
    if (!renderObject.mesh || !renderObject.texture) {
        std::cerr << "Warning: RenderObject missing mesh or texture, skipping" << std::endl;
        return;
    }
    
    // Verify resources exist
    if (meshResources.find(renderObject.mesh) == meshResources.end()) {
        throw std::runtime_error("Mesh not found in resources - was it loaded through LoadMesh?");
    }
    if (textureResources.find(renderObject.texture) == textureResources.end()) {
        throw std::runtime_error("Texture not found in resources - was it loaded through LoadTexture?");
    }
    
    renderQueue.push_back(renderObject);
}

void VulkanDriver::ClearRenderQueue() {
    renderQueue.clear();
}

void VulkanDriver::SetViewMatrix(const glm::mat4& view) {
    viewMatrix = view;
}

void VulkanDriver::SetProjectionMatrix(const glm::mat4& projection) {
    projectionMatrix = projection;
}

VulkanMesh VulkanDriver::CreateVulkanMesh(const Mesh& mesh) {
    VulkanMesh vulkanMesh{};
    const auto& vertices = mesh.GetVertices();
    const auto& indices = mesh.GetIndices();
    
    // Create vertex buffer
    VkDeviceSize vertexBufferSize = sizeof(vertices[0]) * vertices.size();
    
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(device, stagingBufferMemory, 0, vertexBufferSize, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(vertexBufferSize));
    vkUnmapMemory(device, stagingBufferMemory);

    CreateBuffer(vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                 vulkanMesh.vertexBuffer, vulkanMesh.vertexBufferMemory);

    CopyBuffer(stagingBuffer, vulkanMesh.vertexBuffer, vertexBufferSize);
    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);

    // Create index buffer
    VkDeviceSize indexBufferSize = sizeof(indices[0]) * indices.size();
    
    CreateBuffer(indexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                 stagingBuffer, stagingBufferMemory);

    vkMapMemory(device, stagingBufferMemory, 0, indexBufferSize, 0, &data);
    memcpy(data, indices.data(), indexBufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    CreateBuffer(indexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                 vulkanMesh.indexBuffer, vulkanMesh.indexBufferMemory);
    CopyBuffer(stagingBuffer, vulkanMesh.indexBuffer, indexBufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
    
    vulkanMesh.indexCount = static_cast<uint32_t>(indices.size());
    
    return vulkanMesh;
}

void VulkanDriver::DestroyVulkanMesh(VulkanMesh& vulkanMesh) {
    vkDestroyBuffer(device, vulkanMesh.vertexBuffer, nullptr);
    vkFreeMemory(device, vulkanMesh.vertexBufferMemory, nullptr);
    vkDestroyBuffer(device, vulkanMesh.indexBuffer, nullptr);
    vkFreeMemory(device, vulkanMesh.indexBufferMemory, nullptr);
}

VulkanTexture VulkanDriver::CreateVulkanTexture(const std::string& texturePath) {
    VulkanTexture vulkanTexture{};
    
    int texWidth, texHeight, texChannels;
    stbi_uc *pixels = stbi_load(texturePath.c_str(), &texWidth, &texHeight,
                                &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) {
        throw std::runtime_error("Failed to load texture: " + texturePath);
    }

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(device, stagingBufferMemory);
    stbi_image_free(pixels);

    CreateImage(static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight),
                VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                vulkanTexture.image, vulkanTexture.imageMemory);

    TransitionImageLayout(vulkanTexture.image, VK_FORMAT_R8G8B8A8_SRGB,
                          VK_IMAGE_LAYOUT_UNDEFINED,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyBufferToImage(stagingBuffer, vulkanTexture.image,
                      static_cast<uint32_t>(texWidth),
                      static_cast<uint32_t>(texHeight));

    TransitionImageLayout(vulkanTexture.image, VK_FORMAT_R8G8B8A8_SRGB,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
    
    vulkanTexture.imageView = CreateImageView(vulkanTexture.image, VK_FORMAT_R8G8B8A8_SRGB,
                                               VK_IMAGE_ASPECT_COLOR_BIT);
    vulkanTexture.sampler = defaultTextureSampler;
    
    return vulkanTexture;
}

void VulkanDriver::DestroyVulkanTexture(VulkanTexture& vulkanTexture) {
    vkDestroyImageView(device, vulkanTexture.imageView, nullptr);
    vkDestroyImage(device, vulkanTexture.image, nullptr);
    vkFreeMemory(device, vulkanTexture.imageMemory, nullptr);
    // Note: sampler is shared, don't destroy it here
}


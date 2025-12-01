#include "Engine/Graphics/Drivers/Vulkan/Vulkan.h"
#include "Engine/Graphics/Drivers/Vulkan/RenderObject.h"
#include "Engine/Graphics/Drivers/Vulkan/Vertex.h"
#include "Engine/Graphics/GraphicsManager.h"
#include "Engine/Graphics/Drivers/IGraphicsDriver.h"
#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>

bool shouldQuit = false;

void HandleKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		shouldQuit = true;
	}
}

// Generate a cube mesh programmatically
std::pair<std::vector<Vertex>, std::vector<uint32_t>> GenerateCubeMesh(float size = 0.5f) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    
    float s = size;
    
    // Define the 8 vertices of a cube
    std::vector<glm::vec3> positions = {
        {-s, -s, -s}, { s, -s, -s}, { s,  s, -s}, {-s,  s, -s},  // Back face
        {-s, -s,  s}, { s, -s,  s}, { s,  s,  s}, {-s,  s,  s}   // Front face
    };
    
    // Define texture coordinates for each face
    std::vector<glm::vec2> texCoords = {
        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}
    };
    
    // Define the 6 faces of the cube
    struct Face {
        int v0, v1, v2, v3;
    };
    
    std::vector<Face> faces = {
        {0, 1, 2, 3},  // Back
        {4, 7, 6, 5},  // Front
        {0, 4, 5, 1},  // Bottom
        {2, 6, 7, 3},  // Top
        {0, 3, 7, 4},  // Left
        {1, 5, 6, 2}   // Right
    };
    
    uint32_t indexOffset = 0;
    for (const auto& face : faces) {
        // Add 4 vertices for this face
        for (int i = 0; i < 4; i++) {
            int vertexIdx = (i == 0) ? face.v0 : (i == 1) ? face.v1 : (i == 2) ? face.v2 : face.v3;
            Vertex v;
            v.pos = positions[vertexIdx];
            v.color = {1.0f, 1.0f, 1.0f};  // White color
            v.texCoord = texCoords[i];
            vertices.push_back(v);
        }
        
        // Add 2 triangles (6 indices) for this face
        indices.push_back(indexOffset + 0);
        indices.push_back(indexOffset + 1);
        indices.push_back(indexOffset + 2);
        indices.push_back(indexOffset + 2);
        indices.push_back(indexOffset + 3);
        indices.push_back(indexOffset + 0);
        
        indexOffset += 4;
    }
    
    return {vertices, indices};
}

// Generate a rainbow texture programmatically
std::vector<uint8_t> GenerateRainbowTexture(uint32_t width, uint32_t height) {
    std::vector<uint8_t> pixels(width * height * 4);  // RGBA
    
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            uint32_t index = (y * width + x) * 4;
            
            // Create a rainbow pattern based on position
            float fx = static_cast<float>(x) / width;
            float fy = static_cast<float>(y) / height;
            
            // Create rainbow colors using HSV to RGB conversion
            float hue = fx * 360.0f;  // Hue varies from 0 to 360
            float saturation = 1.0f;
            float value = 0.5f + 0.5f * std::sin(fy * 3.14159f * 2.0f);  // Vary brightness
            
            // HSV to RGB conversion
            float c = value * saturation;
            float x_h = c * (1.0f - std::abs(std::fmod(hue / 60.0f, 2.0f) - 1.0f));
            float m = value - c;
            
            float r, g, b;
            if (hue < 60) {
                r = c; g = x_h; b = 0;
            } else if (hue < 120) {
                r = x_h; g = c; b = 0;
            } else if (hue < 180) {
                r = 0; g = c; b = x_h;
            } else if (hue < 240) {
                r = 0; g = x_h; b = c;
            } else if (hue < 300) {
                r = x_h; g = 0; b = c;
            } else {
                r = c; g = 0; b = x_h;
            }
            
            pixels[index + 0] = static_cast<uint8_t>((r + m) * 255.0f);  // R
            pixels[index + 1] = static_cast<uint8_t>((g + m) * 255.0f);  // G
            pixels[index + 2] = static_cast<uint8_t>((b + m) * 255.0f);  // B
            pixels[index + 3] = 255;  // A
        }
    }
    
    return pixels;
}

void GameLoop(GraphicsManager* gManager, IGraphicsDriver* driver, 
              std::shared_ptr<Mesh> loadedMesh, std::shared_ptr<Texture> loadedTexture,
              std::shared_ptr<Mesh> cubeMesh, std::shared_ptr<Texture> rainbowTexture) {
	// Check Input
	// Update entities
	// Render Screen
	// do these while game state is not quitting
	
	static auto startTime = std::chrono::high_resolution_clock::now();
	
	while (!shouldQuit) {
		// Clear render queue each frame
		driver->ClearRenderQueue();
		
		// Update time for animation
		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		
		// Update camera matrices (example: rotating camera)
		glm::mat4 view = glm::lookAt(
			glm::vec3(3.0f * cos(time * 0.3f), 3.0f * sin(time * 0.3f), 2.5f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);
		driver->SetViewMatrix(view);
		
		// Set projection matrix
		int width, height;
		glfwGetFramebufferSize(gManager->getWindow(), &width, &height);
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 20.0f);
		driver->SetProjectionMatrix(proj);
		
		// Submit multiple render objects
		
		// 1. Original loaded mesh at center, rotating on Z axis
		glm::mat4 modelMatrix1 = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		RenderObject obj1(loadedMesh, loadedTexture, modelMatrix1);
		driver->SubmitRenderObject(obj1);
		
		// 2. Cube on the left, rotating on Y axis
		glm::mat4 modelMatrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 0.0f, 0.0f));
		modelMatrix2 = glm::rotate(modelMatrix2, time * glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		RenderObject obj2(cubeMesh, rainbowTexture, modelMatrix2);
		driver->SubmitRenderObject(obj2);
		
		// 3. Cube on the right, rotating on X axis
		glm::mat4 modelMatrix3 = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f));
		modelMatrix3 = glm::rotate(modelMatrix3, time * glm::radians(150.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		RenderObject obj3(cubeMesh, rainbowTexture, modelMatrix3);
		driver->SubmitRenderObject(obj3);
		
		// 4. Cube above, rotating on all axes
		glm::mat4 modelMatrix4 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.5f));
		modelMatrix4 = glm::rotate(modelMatrix4, time * glm::radians(100.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		RenderObject obj4(cubeMesh, rainbowTexture, modelMatrix4);
		driver->SubmitRenderObject(obj4);
		
		// 5. Cube below, rotating on Z and Y axes
		glm::mat4 modelMatrix5 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.5f));
		modelMatrix5 = glm::rotate(modelMatrix5, time * glm::radians(80.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		RenderObject obj5(cubeMesh, rainbowTexture, modelMatrix5);
		driver->SubmitRenderObject(obj5);
		
		gManager->update();
	}
}

int main() {
	VulkanDriver vulkanDriver{};

	// Setup window
	GraphicsManager gManager = GraphicsManager(1024, 768, "Darkest Planet", &vulkanDriver);	
	if (!gManager.isInitialized()) {
		return -1;
	}
	glfwSetKeyCallback(gManager.getWindow(), HandleKey);
	
	// Load resources using the new API
	std::shared_ptr<Mesh> loadedMesh;
	std::shared_ptr<Texture> loadedTexture;
	
	try {
		loadedMesh = vulkanDriver.LoadMesh("models/viking_room.obj");
		loadedTexture = vulkanDriver.LoadTexture("textures/viking_room.png");
	} catch (const std::exception& e) {
		std::cerr << "Failed to load resources: " << e.what() << std::endl;
		return -1;
	}
	
	// Create programmatic resources
	std::cout << "\nCreating programmatic cube mesh..." << std::endl;
	auto [cubeVertices, cubeIndices] = GenerateCubeMesh(0.3f);
	std::shared_ptr<Mesh> cubeMesh = vulkanDriver.CreateMesh(cubeVertices, cubeIndices);
	
	std::cout << "Creating programmatic rainbow texture..." << std::endl;
	const uint32_t textureSize = 256;
	auto rainbowPixels = GenerateRainbowTexture(textureSize, textureSize);
	std::shared_ptr<Texture> rainbowTexture = vulkanDriver.CreateTexture(
		textureSize, textureSize, rainbowPixels.data());
	
	std::cout << "\nRendering " << 5 << " objects:" << std::endl;
	std::cout << "  - 1 loaded mesh (viking room) at center" << std::endl;
	std::cout << "  - 4 programmatic cubes with rainbow textures" << std::endl;
	std::cout << "\nPress ESC to exit\n" << std::endl;
	
	GameLoop(&gManager, &vulkanDriver, loadedMesh, loadedTexture, cubeMesh, rainbowTexture);

	gManager.destroyWindow();
	return 0;
}

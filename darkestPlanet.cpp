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
    
    float s = size * 0.5f; // around origin we need to halve for the right size
    
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

// Generate a grass-like pixelated texture programmatically
std::vector<uint8_t> GenerateGrassTexture(uint32_t width, uint32_t height) {
    std::vector<uint8_t> pixels(width * height * 4);  // RGBA

    // Main grass color
    const uint8_t base_r = 55;
    const uint8_t base_g = 170;
    const uint8_t base_b = 47;

    // For a "pixelated" look, create chunky blocks of grass blades and dirt
    uint32_t blockSize = std::max(2u, width / 16); // controls "pixels" size for pixelated effect

    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            uint32_t index = (y * width + x) * 4;

            // Calculate block coordinates
            uint32_t bx = x / blockSize;
            uint32_t by = y / blockSize;

            // Add variation in green tones for the grass "pixels"
            uint8_t r = base_r + static_cast<uint8_t>((bx * by + x + y) % 13);
            uint8_t g = base_g + static_cast<uint8_t>((bx * 5 + by * 13 + x) % 30); // stronger green
            uint8_t b = base_b + static_cast<uint8_t>((bx * 7 + by * 3 + y) % 14);

            // Optionally: simulate some dirt at the bottom (brownish, last ~15% rows)
            float fy = static_cast<float>(y) / height;
            if (fy > 0.85f) {
                // Brown dirt
                r = 111 + static_cast<uint8_t>((bx * 19 + x) % 14);
                g = 79 + static_cast<uint8_t>((by * 11 + y) % 8);
                b = 44 + static_cast<uint8_t>((bx * 13 + y) % 8);
            }

            pixels[index + 0] = r;
            pixels[index + 1] = g;
            pixels[index + 2] = b;
            pixels[index + 3] = 255; // Alpha
        }
    }

    return pixels;
}

void GameLoop(GraphicsManager* gManager, IGraphicsDriver* driver, 
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
		

		glm::vec3 cameraPos = glm::vec3(3.0f, 3.0f, 2.5f);
		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraDirection = glm::normalize(cameraTarget - cameraPos);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraRight = glm::normalize(glm::cross(cameraDirection, up));	
		glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));
		
		glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
		driver->SetViewMatrix(view);
		
		// Set projection matrix
		int width, height;
		glfwGetFramebufferSize(gManager->getWindow(), &width, &height);
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 20.0f);
		driver->SetProjectionMatrix(proj);
		
		// 1. Cube on the left, rotating on Y axis
		glm::mat4 modelMatrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		modelMatrix2 = glm::rotate(modelMatrix2, time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		RenderObject obj2(cubeMesh, rainbowTexture, modelMatrix2);
		driver->SubmitRenderObject(obj2);
		
		
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
	// std::shared_ptr<Mesh> loadedMesh;
	// std::shared_ptr<Texture> loadedTexture;
	
	// Kept for later reference
	// try {
	// 	loadedMesh = vulkanDriver.LoadMesh("models/viking_room.obj");
	// 	loadedTexture = vulkanDriver.LoadTexture("textures/viking_room.png");
	// } catch (const std::exception& e) {
	// 	std::cerr << "Failed to load resources: " << e.what() << std::endl;
	// 	return -1;
	// }
	
	// Create programmatic resources
	std::cout << "\nCreating programmatic cube mesh..." << std::endl;
	auto [cubeVertices, cubeIndices] = GenerateCubeMesh(0.3f);
	std::shared_ptr<Mesh> cubeMesh = vulkanDriver.CreateMesh(cubeVertices, cubeIndices);
	
	std::cout << "Creating programmatic grass texture..." << std::endl;
	const uint32_t textureSize = 256;
	auto grassPixels = GenerateGrassTexture(textureSize, textureSize);
	std::shared_ptr<Texture> grassTexture = vulkanDriver.CreateTexture(
		textureSize, textureSize, grassPixels.data());
	
	std::cout << "Rendering the game..." << std::endl;
	std::cout << "\nPress ESC to exit\n" << std::endl;
	
	GameLoop(&gManager, &vulkanDriver, cubeMesh, grassTexture);

	gManager.destroyWindow();
	return 0;
}

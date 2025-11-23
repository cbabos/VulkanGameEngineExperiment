#include "Engine/Graphics/Drivers/Vulkan/Vulkan.h"
#include "Engine/Graphics/Drivers/Vulkan/RenderObject.h"
#include "Engine/Graphics/GraphicsManager.h"
#include "Engine/Graphics/Drivers/IGraphicsDriver.h"
#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <iostream>

bool shouldQuit = false;

void HandleKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		shouldQuit = true;
	}
}

void GameLoop(GraphicsManager* gManager, IGraphicsDriver* driver, 
              std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture) {
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
			glm::vec3(2.0f * cos(time * 0.5f), 2.0f * sin(time * 0.5f), 2.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);
		driver->SetViewMatrix(view);
		
		// Set projection matrix
		int width, height;
		glfwGetFramebufferSize(gManager->getWindow(), &width, &height);
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 10.0f);
		driver->SetProjectionMatrix(proj);
		
		// Submit render objects each frame (with updated model matrix)
		glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		RenderObject renderObject(mesh, texture, modelMatrix);
		driver->SubmitRenderObject(renderObject);
		
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
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Texture> texture;
	
	try {
		mesh = vulkanDriver.LoadMesh("models/viking_room.obj");
		texture = vulkanDriver.LoadTexture("textures/viking_room.png");
	} catch (const std::exception& e) {
		std::cerr << "Failed to load resources: " << e.what() << std::endl;
		return -1;
	}
	
	GameLoop(&gManager, &vulkanDriver, mesh, texture);

	gManager.destroyWindow();
	return 0;
}

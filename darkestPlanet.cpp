#include "Engine/Graphics/Drivers/Vulkan/Vulkan.h"
#include "Engine/Graphics/GraphicsManager.h"
#include "GLFW/glfw3.h"

bool shouldQuit = false;

void HandleKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		shouldQuit = true;
	}
}

void GameLoop(GraphicsManager* gManager) {
	// Check Input
	// Update entities
	// Render Screen
	// do these while game state is not quitting
	while (!shouldQuit) {
		gManager->update();
	}
	
}

int main() {
	VulkanDriver vulkanDriver{};

	// Setup window
	// Setup input
	GraphicsManager gManager = GraphicsManager(1024, 768, "Darkest Planet", &vulkanDriver);	
	if (!gManager.isInitialized()) {
		return -1;
	}
	glfwSetKeyCallback(gManager.getWindow(), HandleKey);

	GameLoop(&gManager);

	gManager.destroyWindow();
	return 0;
}

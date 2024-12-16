#include "Engine/Graphics/Drivers/Vulkan/Vulkan.h"
//#include "Engine/Graphics/Drivers/Dummy/Dummy.h"
#include "Engine/Graphics/GraphicsManager.h"
#include "GLFW/glfw3.h"
#include <iostream>
using namespace std;

bool shouldQuit = false;

void HandleKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		cout << "Wish to quit" << endl;
		shouldQuit = true;
	}
}

void GameLoop(GraphicsManager* gManager) {
	cout << "This will be the main game loop" << endl;

	// Check Input
	// Update entities
	// Render Screen
	// do these while game state is not quitting
	while (!shouldQuit) {
		gManager->update();
	}
	
}

int main() {
	cout << "Darkest Planet v0.0" << endl << "===================" << endl; 
	VulkanDriver vulkanDriver{};
	//DummyDriver dummyDriver{};

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

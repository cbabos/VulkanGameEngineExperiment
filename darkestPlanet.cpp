#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

#include "GraphicsManager.h"

bool shouldQuit = false;

void HandleKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		cout << "Wish to quit" << endl;
		shouldQuit = true;
	}
}

void GameLoop(GraphicsManager gManager) {
	cout << "This will be the main game loop" << endl;

	// Check Input
	// Update entities
	// Render Screen
	// do these while game state is not quitting
	while (!shouldQuit) {
		gManager.update();
        glfwPollEvents();
	}
	
}

int main() {
	cout << "Darkest Planet v0.0" << endl << "===================" << endl; 

	// Setup window
	// Setup input
	GraphicsManager gManager = GraphicsManager(1024, 768, "Darkest Planet");	
	glfwSetKeyCallback(gManager.getWindow(), HandleKey);

	GameLoop(gManager);

	gManager.destroyWindow();

	return 0;
}

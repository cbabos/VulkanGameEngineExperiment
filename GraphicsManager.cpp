#include "GLFW/glfw3.h"
#include <iostream>
using namespace std;

#include "GraphicsManager.h"

GraphicsManager::GraphicsManager(int width, int height, const char* title) {
	if (!glfwInit()) {
		cerr << "Failed to initialize GLFW" << endl;
		window = nullptr;
		return;
	}

	cout << "Creating new window [" << width << "x" << height << " | " 
		 << title << endl;

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window) {
		cerr << "Failed to create window" << endl;
		glfwTerminate();
		return;
	}

    glfwMakeContextCurrent(window);
}

GLFWwindow* GraphicsManager::getWindow() const {
	return window;
}

void GraphicsManager::destroyWindow() {
	cout << "Destroying main window..." << endl;
	if (window) { 
		glfwDestroyWindow(window);
		window = nullptr;
	}
	glfwTerminate();
}

bool GraphicsManager::shouldClose() {
	return glfwWindowShouldClose(window);
}

void GraphicsManager::update() {
	glfwSwapBuffers(window);
}

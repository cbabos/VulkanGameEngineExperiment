#include <iostream>
#include "GraphicsManager.h"
#include "Drivers/IGraphicsDriver.h"
#include "GLFW/glfw3.h"

bool GraphicsManager::isInitialized() {
	return isManagerUp;
}

GraphicsManager::GraphicsManager(int width, int height, const char* title, IGraphicsDriver* selectedDriver) {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		window = nullptr;
		return;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // No OpenGL please 

	std::cout << "Creating new window [" << width << "x" << height << "] | " 
		<< title << std::endl;

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window) {
		std::cerr << "Failed to create window" << std::endl;
		glfwTerminate();
		return;
	}

	try {
		// Should init driver here
		//driver.reset(selectedDriver);
		driver = selectedDriver;
		driver->Setup(window);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return;
	}

	glfwMakeContextCurrent(window);
	isManagerUp = GLFW_TRUE;
}

GLFWwindow* GraphicsManager::getWindow() const {
	return window;
}

void GraphicsManager::destroyWindow() {
	// should destroy driver here
	driver->Destruct();
	
	std::cout << "Destroying main window..." << std::endl;
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
	driver->RenderFrame();
	glfwPollEvents();
}

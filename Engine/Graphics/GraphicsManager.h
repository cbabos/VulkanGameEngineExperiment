#include "Drivers/IGraphicsDriver.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>

class GraphicsManager {
	protected:
	GLFWwindow* window; 
	bool isManagerUp = false;
	//std::unique_ptr<IGraphicsDriver> driver = nullptr;
	IGraphicsDriver* driver = nullptr;

	public: 
	GraphicsManager(int width, int height, const char* title, IGraphicsDriver* driver);
	GLFWwindow* getWindow() const;
	bool shouldClose();
	bool isInitialized();
	void update();
	void destroyWindow();
}; 

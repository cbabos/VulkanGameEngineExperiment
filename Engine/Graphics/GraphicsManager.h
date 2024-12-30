#include "Drivers/IGraphicsDriver.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class GraphicsManager {
	private:
	GLFWwindow* window; 
	bool isManagerUp = false;
	IGraphicsDriver* driver = nullptr;

	public: 
	static void ResizeCallback(GLFWwindow* window, int newWidth, int newHeight);
	GraphicsManager(int width, int height, const char* title, IGraphicsDriver* driver);
	GLFWwindow* getWindow() const;
	bool shouldClose();
	bool isInitialized();
	void update();
	void destroyWindow();
}; 

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class GraphicsManager {
	protected:
	GLFWwindow* window; 
	VkInstance instance;
	bool isManagerUp = false;
	
	bool CreateVulkanInstance();
	void DestroyVulkanInstance();


	public: 
	GraphicsManager(int width, int height, const char* title);
	GLFWwindow* getWindow() const;
	bool shouldClose();
	bool isInitialized();
	void update();
	void destroyWindow();
}; 

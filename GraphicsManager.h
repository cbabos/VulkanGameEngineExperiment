#include <GLFW/glfw3.h>

class GraphicsManager {
	protected:
		GLFWwindow* window; 

	public: 
	GraphicsManager(int width, int height, const char* title);
	GLFWwindow* getWindow() const;
	bool shouldClose();
	void update();
	void destroyWindow();
}; 

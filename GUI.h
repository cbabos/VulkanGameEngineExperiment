#include <GLFW/glfw3.h>

class GUI {
	protected:
		GLFWwindow* window; 

	public: 
	GUI(int width, int height, const char* title);
	GLFWwindow* getWindow() const;
	bool shouldClose();
	void update();
	void destroyWindow();
}; 

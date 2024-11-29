#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <iostream>
using namespace std;

#include "GUI.h"

void GameLoop(GUI gui) {
	cout << "This will be the main game loop" << endl;

	// Check Input
	// Update entities
	// Render Screen
	// do these while game state is not quitting
	while (!gui.shouldClose()) {
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(-0.5f, -0.5f, 0.0f);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(0.0f, .5f, 0.0f);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(.5f, -0.5f, 0.0f);
		glEnd();


		gui.update();
        glfwPollEvents();
	}
	
}

int main() {
	cout << "Darkest Planet v0.0" << endl << "===================" << endl; 

	// Setup window
	// Setup input
	GUI gui = GUI(640, 480, "Darkest Planet");	

	GameLoop(gui);

	gui.destroyWindow();

	return 0;
}

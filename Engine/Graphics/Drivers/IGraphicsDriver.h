#ifndef IGRAPHICSDRIVER_H
#define IGRAPHICSDRIVER_H

#include "GLFW/glfw3.h"
class IGraphicsDriver {
public: 
	virtual ~IGraphicsDriver() = default;

	virtual void Setup(GLFWwindow* window) = 0;
	virtual void Destruct() = 0;
	virtual void RenderFrame() = 0;
};

#endif

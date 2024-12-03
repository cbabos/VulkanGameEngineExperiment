#ifndef DUMMYDRIVER_H
#define DUMMYDRIVER_H 
#include <iostream>
#include "../IGraphicsDriver.h"

class DummyDriver : public IGraphicsDriver {
	public: 
		DummyDriver();
		~DummyDriver();

		void Setup(GLFWwindow* window) override;
		void Destruct() override;
		void RenderFrame() override;

	private: 
		GLFWwindow* window;
};

#endif

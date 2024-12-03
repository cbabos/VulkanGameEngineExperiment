#include "Dummy.h"

DummyDriver::DummyDriver() {}
DummyDriver::~DummyDriver() {}
void DummyDriver::RenderFrame() {}

void DummyDriver::Setup(GLFWwindow* engineWindow) {
	window = engineWindow;
}

void DummyDriver::Destruct() {}

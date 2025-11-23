#ifndef IGRAPHICSDRIVER_H
#define IGRAPHICSDRIVER_H

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <string>

// Forward declarations
class Mesh;
class Texture;
struct RenderObject;

class IGraphicsDriver {
public: 
	virtual ~IGraphicsDriver() = default;

	virtual void Setup(GLFWwindow* window) = 0;
	virtual void Destruct() = 0;
	virtual void RenderFrame() = 0;
	virtual void WindowIsResized() = 0;
	
	// Resource loading API
	virtual std::shared_ptr<Mesh> LoadMesh(const std::string& modelPath) = 0;
	virtual std::shared_ptr<Texture> LoadTexture(const std::string& texturePath) = 0;
	
	// Render queue API
	virtual void SubmitRenderObject(const RenderObject& renderObject) = 0;
	virtual void ClearRenderQueue() = 0;
	
	// Camera API
	virtual void SetViewMatrix(const glm::mat4& view) = 0;
	virtual void SetProjectionMatrix(const glm::mat4& projection) = 0;
};

#endif

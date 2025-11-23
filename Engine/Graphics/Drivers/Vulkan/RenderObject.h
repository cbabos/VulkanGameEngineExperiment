#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class Mesh;
class Texture;

// RenderObject represents a single object to be rendered
// Contains mesh, transform, and texture reference
struct RenderObject {
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Texture> texture;
    glm::mat4 modelMatrix;  // Model transformation matrix
    
    RenderObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture, const glm::mat4& modelMatrix = glm::mat4(1.0f))
        : mesh(mesh), texture(texture), modelMatrix(modelMatrix) {}
};

#endif // RENDEROBJECT_H


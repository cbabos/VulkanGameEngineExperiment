#ifndef MESH_H
#define MESH_H

#include "Vertex.h"
#include <vector>
#include <cstdint>

// Mesh represents a loaded 3D model with vertices and indices
class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    ~Mesh();

    const std::vector<Vertex>& GetVertices() const { return vertices; }
    const std::vector<uint32_t>& GetIndices() const { return indices; }
    size_t GetVertexCount() const { return vertices.size(); }
    size_t GetIndexCount() const { return indices.size(); }

private:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

#endif // MESH_H


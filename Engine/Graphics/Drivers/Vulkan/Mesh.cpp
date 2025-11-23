#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    : vertices(vertices), indices(indices) {
}

Mesh::~Mesh() {
}


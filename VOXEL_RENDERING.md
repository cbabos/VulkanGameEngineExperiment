# Voxel Rendering Implementation Guide

This document outlines approaches and ideas for implementing voxel rendering in the Darkest Planet engine.

## Overview

Voxel rendering involves representing 3D space as a grid of cubic blocks (voxels) and efficiently rendering only the visible faces. This is commonly used in games like Minecraft, Voxatron, and Teardown.

## Current Engine Capabilities

The engine already has the necessary foundation:
- ✅ Vertex and index buffer support
- ✅ Texture support (can texture voxel faces)
- ✅ Basic rendering pipeline
- ✅ Depth testing

## Implementation Approaches

### Phase 1: Basic Voxel Rendering (Naive Approach)

**Goal:** Get voxels rendering on screen quickly

**Steps:**
1. **Voxel Data Structure**
   ```cpp
   struct Voxel {
       uint8_t type;  // 0 = air, 1+ = different block types
       // Could add: color, texture ID, etc.
   };
   
   class VoxelChunk {
       static const int CHUNK_SIZE = 16;  // 16x16x16 voxels
       Voxel voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
       
       // Generate mesh from voxel data
       void GenerateMesh(std::vector<Vertex>& vertices, 
                        std::vector<uint32_t>& indices);
   };
   ```

2. **Naive Mesh Generation**
   - For each voxel, check its 6 neighbors
   - If a face is exposed (neighbor is air), add a quad
   - Simple but generates many vertices
   - Good starting point for learning

3. **Integration with Current Engine**
   - Create `VoxelRenderer` class that uses `VulkanDriver`
   - Generate mesh on CPU (initially)
   - Upload to existing vertex/index buffer system
   - Use existing texture system for voxel textures

**Pros:**
- Simple to implement
- Easy to understand
- Works with current engine architecture

**Cons:**
- Inefficient (many duplicate vertices)
- CPU-bound mesh generation
- Not suitable for large worlds

### Phase 2: Optimized Mesh Generation

**Goal:** Reduce vertex count and improve performance

**Approach: Greedy Meshing**
- Combine adjacent faces of the same type into larger quads
- Reduces vertex count by 50-90%
- Still CPU-based but much more efficient

**Implementation:**
1. Scan voxel grid in 2D slices
2. Find rectangular regions of same voxel type
3. Combine into single quad
4. Repeat for all 6 directions

**Resources:**
- [0fps Greedy Meshing](https://0fps.net/2012/06/30/meshing-in-a-minecraft-game/)
- [Mikola Lysenko's Greedy Meshing](https://github.com/mikolalysenko/mikolalysenko.github.com/blob/master/Isosurface/js/greedy.js)

### Phase 3: Chunk-Based System

**Goal:** Handle large voxel worlds efficiently

**Architecture:**
```
World
  └─> ChunkManager
       └─> VoxelChunk[] (16x16x16 or 32x32x32)
            └─> Mesh (generated from voxels)
```

**Features:**
- Load/unload chunks based on player position
- Generate meshes only for loaded chunks
- Update meshes when chunks are modified
- Frustum culling (don't render chunks outside view)

**Implementation:**
1. **Chunk Manager**
   ```cpp
   class ChunkManager {
       std::unordered_map<ChunkCoord, VoxelChunk> chunks;
       
       void UpdateChunks(glm::vec3 playerPos);
       void LoadChunk(ChunkCoord coord);
       void UnloadChunk(ChunkCoord coord);
   };
   ```

2. **Coordinate System**
   - World coordinates (continuous)
   - Chunk coordinates (discrete)
   - Local voxel coordinates (within chunk)

3. **Mesh Updates**
   - Mark chunks as "dirty" when voxels change
   - Regenerate mesh only for dirty chunks
   - Use background thread for mesh generation (optional)

### Phase 4: Advanced Optimizations

**GPU-Based Mesh Generation (Advanced)**
- Use compute shaders to generate meshes on GPU
- Much faster for large worlds
- Requires Vulkan compute pipeline

**Instanced Rendering**
- For voxels of same type, use instanced rendering
- Reduces draw calls significantly
- Good for repetitive structures

**Level of Detail (LOD)**
- Generate lower detail meshes for distant chunks
- Reduce vertex count for far-away areas
- Smooth transitions between LOD levels

**Occlusion Culling**
- Don't render chunks completely hidden by others
- Use hierarchical Z-buffer or similar techniques

## Recommended Implementation Path

### Step 1: Simple Voxel Renderer (Week 1-2)
1. Create basic `Voxel` and `VoxelChunk` classes
2. Implement naive mesh generation
3. Render single chunk with different voxel types
4. Add simple texture atlas for different block types

**Deliverable:** Single chunk rendering with multiple voxel types

### Step 2: Multiple Chunks (Week 3-4)
1. Implement `ChunkManager`
2. Load multiple chunks around origin
3. Add basic camera movement
4. Implement chunk loading/unloading

**Deliverable:** Small world with multiple chunks

### Step 3: Greedy Meshing (Week 5-6)
1. Implement greedy meshing algorithm
2. Compare performance with naive approach
3. Add mesh caching (regenerate only when chunk changes)

**Deliverable:** Optimized mesh generation

### Step 4: World Generation (Week 7-8)
1. Add procedural world generation
2. Implement basic terrain generation (heightmap, noise)
3. Add different voxel types (grass, dirt, stone, etc.)

**Deliverable:** Procedurally generated voxel world

### Step 5: Interaction (Week 9-10)
1. Add voxel placement (click to place)
2. Add voxel removal (click to destroy)
3. Update chunks when voxels change

**Deliverable:** Interactive voxel world

## Integration with Current Engine

### Required Changes

1. **Resource Management**
   - Extend current model loading to support voxel meshes
   - Add texture atlas support for multiple block types
   - Create `VoxelMesh` class that generates vertices/indices

2. **Rendering System**
   - Extend `VulkanDriver` or create `VoxelRenderer` wrapper
   - Support multiple chunks in single render call
   - Add frustum culling

3. **Camera System**
   - Abstract current hardcoded camera
   - Add first-person camera controls
   - Implement view frustum for culling

4. **Scene Management**
   - Create scene graph or render queue
   - Support multiple renderable objects (chunks)
   - Batch similar chunks together

### Code Structure Suggestion

```
Engine/
  ├── Graphics/
  │   ├── Drivers/Vulkan/  (existing)
  │   └── Voxel/
  │       ├── Voxel.h
  │       ├── VoxelChunk.h
  │       ├── VoxelChunk.cpp
  │       ├── ChunkManager.h
  │       ├── ChunkManager.cpp
  │       ├── VoxelMeshGenerator.h
  │       ├── VoxelMeshGenerator.cpp
  │       └── VoxelRenderer.h
  ├── World/
  │   ├── WorldGenerator.h
  │   └── WorldGenerator.cpp
  └── Camera/
      ├── Camera.h
      └── Camera.cpp
```

## Shader Considerations

### Current Shaders
The existing shaders can work for voxels with minimal changes:
- Vertex shader: Already handles MVP matrices
- Fragment shader: Already handles textures

### Enhancements for Voxels
1. **Texture Atlas**
   - Modify fragment shader to sample from texture atlas
   - Use UV coordinates to select correct texture region
   - Add block type to vertex data or use uniform

2. **Lighting**
   - Add simple ambient + directional lighting
   - Face-based lighting (different brightness per face)
   - Or per-vertex lighting

3. **Shadows** (Advanced)
   - Shadow mapping for voxel shadows
   - Or simple vertex-based shadow calculation

## Performance Targets

**Initial Goals:**
- 60 FPS with 10-20 chunks (16x16x16 each)
- Smooth camera movement
- Responsive voxel placement/removal

**Optimization Goals:**
- 60 FPS with 100+ chunks
- Large view distance
- Fast mesh regeneration

## Testing Strategy

1. **Unit Tests**
   - Mesh generation correctness
   - Chunk coordinate conversion
   - Greedy meshing algorithm

2. **Performance Tests**
   - Frame time measurements
   - Vertex count tracking
   - Memory usage monitoring

3. **Visual Tests**
   - Different voxel types rendering correctly
   - Chunk boundaries seamless
   - Texture atlas working properly

## Resources and References

**Voxel Rendering:**
- [0fps Voxel Engine Series](https://0fps.net/category/programming/voxel-worlds/)
- [Voxel Game Engine Tutorial](https://github.com/BSVino/MathForGameDevs)
- [Greedy Meshing Algorithm](https://0fps.net/2012/06/30/meshing-in-a-minecraft-game/)

**Vulkan Optimization:**
- [Vulkan Best Practices](https://github.com/KhronosGroup/Vulkan-Samples)
- [Vulkan Performance Guide](https://developer.nvidia.com/vulkan-shader-resource-binding)

**Open Source Voxel Engines:**
- [Minetest](https://github.com/minetest/minetest) - C++ voxel engine
- [Veloren](https://github.com/veloren/veloren) - Rust voxel engine
- Study their mesh generation approaches

## Next Steps

1. Review this document and choose implementation path
2. Start with Phase 1 (Simple Voxel Renderer)
3. Integrate with current engine architecture
4. Iterate and optimize based on performance

Good luck with your voxel rendering implementation! 🎮


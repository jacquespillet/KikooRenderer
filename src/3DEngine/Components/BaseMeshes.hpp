#pragma once
#include "Util/Common.h"
#include "MeshFilterComponent.hpp"

namespace KikooRenderer {
namespace CoreEngine {

    void GetCubeBuffers(std::vector<glm::vec3>* vertex, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uv, std::vector<glm::vec4>* colors, std::vector<int>* triangles);
    void GetSphereBuffers(std::vector<glm::vec3>* vertex, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uv, std::vector<glm::vec4>* colors, std::vector<int>* triangles, int numSlices = 32);
    void GetCapsuleBuffers(std::vector<glm::vec3>* vertex, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uv, std::vector<glm::vec4>* colors, std::vector<int>* triangles, float height = 4, float radius=1);
    void GetCircleBuffers(std::vector<glm::vec3>* vertex, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uv, std::vector<glm::vec4>* colors, std::vector<int>* triangles);
    void GetWireCircleBuffers(std::vector<glm::vec3>* vertex, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uv, std::vector<glm::vec4>* colors, std::vector<int>* triangles);
    void GetConeBuffers(std::vector<glm::vec3>* vertex, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uv, std::vector<glm::vec4>* colors, std::vector<int>* triangles);
    void GetCylinderBuffers(std::vector<glm::vec3>* vertex, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uv, std::vector<glm::vec4>* colors, std::vector<int>* triangles, float height = 4, float radius=1);
    void GetQuadBuffers(std::vector<glm::vec3>* vertex, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uv, std::vector<glm::vec4>* colors, std::vector<int>* triangles);
    void GetMiniQuadBuffers(std::vector<glm::vec3>* vertex, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uv, std::vector<glm::vec4>* colors, std::vector<int>* triangles);

    MeshFilterComponent* GetCubeMesh(glm::vec3 size, glm::vec4 color, Object3D* object);
    MeshFilterComponent* GetSphereMesh(glm::vec3 size, glm::vec4 color, Object3D* object, int numSlices = 32);
    MeshFilterComponent* GetCapsuleMesh(glm::vec3 size, glm::vec4 color, Object3D* object, float height = 4);
    MeshFilterComponent* GetCircleMesh(glm::vec3 size, glm::vec4 color, Object3D* object);
    MeshFilterComponent* GetWireCircleMesh(glm::vec3 size, glm::vec4 color, Object3D* object);
    MeshFilterComponent* GetConeMesh(glm::vec3 size, glm::vec4 color, Object3D* object);
    MeshFilterComponent* GetCylinderMesh(glm::vec3 size, glm::vec4 color, Object3D* object, float height = 4, float radius=1);
    MeshFilterComponent* GetQuadMesh(glm::vec3 size, glm::vec4 color, Object3D* object);
    MeshFilterComponent* GetMiniQuadMesh(glm::vec3 size, glm::vec4 color, Object3D* object); //For debugging FBOs

    MeshFilterComponent* GetById(PRIMITIVE_MESH meshId, Object3D* object);
}
}
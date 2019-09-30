#pragma once
#include "Util/Common.h"
#include "MeshFilterComponent.hpp"

namespace KikooRenderer {
namespace CoreEngine {

    void GetCubeBuffers(std::vector<glm::vec3>* vertex, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uv, std::vector<glm::vec4>* colors, std::vector<int>* triangles);
    void GetSphereBuffers(std::vector<glm::vec3>* vertex, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uv, std::vector<glm::vec4>* colors, std::vector<int>* triangles);
    void GetCircleBuffers(std::vector<glm::vec3>* vertex, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uv, std::vector<glm::vec4>* colors, std::vector<int>* triangles);
    void GetWireCircleBuffers(std::vector<glm::vec3>* vertex, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uv, std::vector<glm::vec4>* colors, std::vector<int>* triangles);
    void GetConeBuffers(std::vector<glm::vec3>* vertex, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uv, std::vector<glm::vec4>* colors, std::vector<int>* triangles);
    void GetQuadBuffers(std::vector<glm::vec3>* vertex, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uv, std::vector<glm::vec4>* colors, std::vector<int>* triangles);
    void GetMiniQuadBuffers(std::vector<glm::vec3>* vertex, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uv, std::vector<glm::vec4>* colors, std::vector<int>* triangles);

    MeshFilterComponent* GetCubeMesh(glm::vec3 size, glm::vec4 color, Object3D* object);
    MeshFilterComponent* GetSphereMesh(glm::vec3 size, glm::vec4 color, Object3D* object);
    MeshFilterComponent* GetCircleMesh(glm::vec3 size, glm::vec4 color, Object3D* object);
    MeshFilterComponent* GetWireCircleMesh(glm::vec3 size, glm::vec4 color, Object3D* object);
    MeshFilterComponent* GetConeMesh(glm::vec3 size, glm::vec4 color, Object3D* object);
    MeshFilterComponent* GetQuadMesh(glm::vec3 size, glm::vec4 color, Object3D* object);
    MeshFilterComponent* GetMiniQuadMesh(glm::vec3 size, glm::vec4 color, Object3D* object); //For debugging FBOs

    MeshFilterComponent* GetById(PRIMITIVE_MESH meshId, Object3D* object);
}
}
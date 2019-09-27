#pragma once
#include "Util/Common.h"
#include "MeshFilterComponent.hpp"

namespace KikooRenderer {
namespace CoreEngine {

    void GetCubeBuffers(std::vector<glm::dvec3>* vertex, std::vector<glm::dvec3>* normals, std::vector<glm::dvec2>* uv, std::vector<glm::dvec4>* colors, std::vector<int>* triangles);
    void GetSphereBuffers(std::vector<glm::dvec3>* vertex, std::vector<glm::dvec3>* normals, std::vector<glm::dvec2>* uv, std::vector<glm::dvec4>* colors, std::vector<int>* triangles);
    void GetCircleBuffers(std::vector<glm::dvec3>* vertex, std::vector<glm::dvec3>* normals, std::vector<glm::dvec2>* uv, std::vector<glm::dvec4>* colors, std::vector<int>* triangles);
    void GetWireCircleBuffers(std::vector<glm::dvec3>* vertex, std::vector<glm::dvec3>* normals, std::vector<glm::dvec2>* uv, std::vector<glm::dvec4>* colors, std::vector<int>* triangles);
    void GetConeBuffers(std::vector<glm::dvec3>* vertex, std::vector<glm::dvec3>* normals, std::vector<glm::dvec2>* uv, std::vector<glm::dvec4>* colors, std::vector<int>* triangles);
    void GetQuadBuffers(std::vector<glm::dvec3>* vertex, std::vector<glm::dvec3>* normals, std::vector<glm::dvec2>* uv, std::vector<glm::dvec4>* colors, std::vector<int>* triangles);
    void GetMiniQuadBuffers(std::vector<glm::dvec3>* vertex, std::vector<glm::dvec3>* normals, std::vector<glm::dvec2>* uv, std::vector<glm::dvec4>* colors, std::vector<int>* triangles);

    MeshFilterComponent* GetCubeMesh(glm::dvec3 size, glm::dvec4 color, Object3D* object);
    MeshFilterComponent* GetSphereMesh(glm::dvec3 size, glm::dvec4 color, Object3D* object);
    MeshFilterComponent* GetCircleMesh(glm::dvec3 size, glm::dvec4 color, Object3D* object);
    MeshFilterComponent* GetWireCircleMesh(glm::dvec3 size, glm::dvec4 color, Object3D* object);
    MeshFilterComponent* GetConeMesh(glm::dvec3 size, glm::dvec4 color, Object3D* object);
    MeshFilterComponent* GetQuadMesh(glm::dvec3 size, glm::dvec4 color, Object3D* object);
    MeshFilterComponent* GetMiniQuadMesh(glm::dvec3 size, glm::dvec4 color, Object3D* object); //For debugging FBOs

    MeshFilterComponent* GetById(PRIMITIVE_MESH meshId, Object3D* object);
}
}
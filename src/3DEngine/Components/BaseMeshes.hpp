#pragma once
#include "Util/Common.h"
#include "MeshFilterComponent.hpp"

namespace KikooRenderer {
namespace CoreEngine {
    enum PRIMITIVE_MESH {CUBE_MESH, SPHERE_MESH, CIRCLE_MESH, CONE_MESH, QUAD_MESH};

    MeshFilterComponent* GetCubeMesh(glm::dvec3 size, glm::dvec4 color, Object3D* object);
    MeshFilterComponent* GetSphereMesh(glm::dvec3 size, glm::dvec4 color, Object3D* object);
    MeshFilterComponent* GetCircleMesh(glm::dvec3 size, glm::dvec4 color, Object3D* object);
    MeshFilterComponent* GetConeMesh(glm::dvec3 size, glm::dvec4 color, Object3D* object);
    MeshFilterComponent* GetQuadMesh(glm::dvec3 size, glm::dvec4 color, Object3D* object);

    MeshFilterComponent* GetById(PRIMITIVE_MESH meshId, Object3D* object);
}
}
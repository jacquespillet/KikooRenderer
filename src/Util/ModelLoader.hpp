#pragma once

#include "Util/Common.h"
#include "3DEngine/Object3D.hpp"

namespace KikooRenderer {
namespace Util {
namespace FileIO {
    CoreEngine::Object3D* ObjectFromOBJ(std::string filename, KikooRenderer::CoreEngine::Scene* scene);
    
    void GetModelBuffers(std::string filename, std::vector<glm::dvec3>* vertex, std::vector<glm::dvec3>* normals, std::vector<glm::dvec2>* uv, std::vector<glm::dvec4>* colors, std::vector<int>* triangles);

}
}
}
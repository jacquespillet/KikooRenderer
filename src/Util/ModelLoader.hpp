#pragma once

#include "Common/Common.h"
#include "3DEngine/Object3D.hpp"
#include "3DEngine/Object3D.hpp"

namespace KikooRenderer {
namespace Util {
namespace FileIO {
    KikooRenderer::CoreEngine::Object3D* ObjectFromOBJ(std::string filename, KikooRenderer::CoreEngine::Scene* scene);
}
}
}
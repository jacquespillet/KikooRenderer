#pragma once

#include "Util/Common.h"
#include "3DEngine/Object3D.hpp"

namespace KikooRenderer {
namespace Util {
namespace FileIO {
    CoreEngine::Object3D* ObjectFromOBJ(std::string filename, KikooRenderer::CoreEngine::Scene* scene);
}
}
}
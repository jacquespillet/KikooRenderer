#pragma once 
#include "Util/Common.h"
#include "3DEngine/Scene.hpp"

namespace KikooRenderer {
    void SceneToFile(CoreEngine::Scene* scene, std::string fileName);

    void SceneFromFile(CoreEngine::Scene* scene, std::string fileName);
}
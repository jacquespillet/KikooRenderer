#pragma once
#include "Common/Common.h"
#include "3DEngine/CameraScene.hpp"


namespace KikooRenderer
{
namespace CoreEngine
{
namespace Util {
    bool RayBoxTest(glm::dvec3 rayOrig, glm::dvec3 rayDir, glm::dmat4 transform, glm::dvec3 minScale,glm::dvec3 maxScale, double& distance);

    
    bool cameraBoxTest(CameraScene& camera, glm::dvec3 position, glm::dvec3 size);
}
}
}
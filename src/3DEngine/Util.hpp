#pragma once
#include "Util/Common.h"
#include "CameraScene.hpp"
#include "Components/TransformComponent.hpp"


namespace KikooRenderer
{
namespace CoreEngine
{
namespace Util {
    bool RayBoxTest(glm::dvec3 rayOrig, glm::dvec3 rayDir, glm::dmat4 transform, glm::dvec3 minScale,glm::dvec3 maxScale, double& distance);

    
    bool CameraBoxTest(CameraScene& camera, TransformComponent* transform);
}
}
}
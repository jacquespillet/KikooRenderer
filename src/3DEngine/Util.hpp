#pragma once
#include "Util/Common.h"
#include "CameraScene.hpp"
#include "Components/TransformComponent.hpp"


namespace KikooRenderer
{
namespace CoreEngine
{
namespace Util {
    bool RayBoxTest(glm::vec3 rayOrig, glm::vec3 rayDir, glm::mat4 transform, glm::vec3 minScale,glm::vec3 maxScale, double& distance);

	bool RayWireCircleTest(glm::vec3 rayOrig, glm::vec3 rayDir, glm::mat4 transform, float radius, double& distance);
    
    bool CameraBoxTest(CameraScene& camera, TransformComponent* transform);
}
}
}
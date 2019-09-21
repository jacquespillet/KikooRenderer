#pragma once

#include "Util/Common.h"

namespace KikooRenderer {
namespace Util {
    float GetRand();

    float GenerateValue(float average, float errorMargin);

    glm::vec3 GenerateRandomUnitVector();

    glm::vec3 GenerateRandomUnitVectorWithinCone(glm::vec3 coneDirection, float angle);
}
}
#pragma once

#include "Common/Common.h"

namespace KikooRenderer {
namespace Geometry {
class Ray {
    public:
    Ray(){}
    Ray(glm::dvec3 origin, glm::dvec3 direction);
    glm::dvec3 origin; 
    glm::dvec3 direction; 
    glm::dvec3 pointAtPosition(double t);
};
}
}
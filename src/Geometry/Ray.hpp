#pragma once

#include "Common/Common.h"

namespace KikooRenderer {
namespace Geometry {
class Ray {
    public:
    Ray(){}
    glm::dvec3 origin; 
    glm::dvec3 direction; 
};
}
}
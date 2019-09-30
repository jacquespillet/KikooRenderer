#pragma once

#include "Util/Common.h"

namespace KikooRenderer {
namespace Geometry {
class Ray {
    public:
    Ray(){}
    Ray(glm::vec3 origin, glm::vec3 direction, float time = 0.0);
    glm::vec3 origin; 
    glm::vec3 direction; 
    glm::vec3 pointAtPosition(double t);
    float time;
    
};
}
}
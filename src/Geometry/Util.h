#pragma once
#include "Common/Common.h"

namespace KikooRenderer{
namespace Geometry {

    glm::dvec3 randomInSphere();
    glm::dvec3 RandomInDisk();

    double Schlick(double cosine, double inx);

    bool Refract(glm::vec3 v, glm::dvec3 n, double niOverNt, glm::dvec3& refracted);
}
}
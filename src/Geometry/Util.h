#pragma once
#include "Util/Common.h"

namespace KikooRenderer{
namespace Geometry {
    double RandomInRange(double a, double b);
    glm::vec3 randomInSphere();
    glm::vec3 randomOnSphere();
    glm::vec3 RandomInDisk();
    glm::vec3 RandomCosineDirection();
    glm::vec3 RandomCosineDirection(float r1, float r2);

    double Schlick(double cosine, double inx);

    bool Refract(glm::vec3 v, glm::vec3 n, double niOverNt, glm::vec3& refracted);
}
}
#pragma once
#include "Util/Common.h"

namespace KikooRenderer{
namespace Geometry {

    glm::vec3 randomInSphere();
    glm::vec3 RandomInDisk();

    double Schlick(double cosine, double inx);

    bool Refract(glm::vec3 v, glm::vec3 n, double niOverNt, glm::vec3& refracted);
}
}
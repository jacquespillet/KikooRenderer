#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"

namespace KikooRenderer {
namespace OfflineRenderer {
class Bounds {
public:
    glm::vec3 min;
    glm::vec3 max;
    glm::mat4 transform;
    glm::mat4 inverseTransform;

    Bounds();
    Bounds(std::vector<glm::vec3> vertices, glm::mat4 transform = glm::mat4(1));
    Bounds(std::vector<Bounds> bounds);
    Bounds(Bounds b0, Bounds b1);

    glm::vec3 GetCenter();
    glm::vec3 GetSize();

    double Hit(KikooRenderer::Geometry::Ray ray);
};
}
}
#pragma once
#include "Shape.hpp"
#include "Geometry/Ray.hpp"
#include "../Materials/Material.hpp"

namespace KikooRenderer {
namespace OfflineRenderer {

class Material;
class Sphere : public Shape {
    public:
        glm::vec3 position;
        double radius;
        Material* material;
        Sphere(glm::vec3 position, double radius, Material* material);
        double HitRay(KikooRenderer::Geometry::Ray ray, double tMin, double tMax, Point& hitPoint);
        glm::vec3 GetPosition(double time);

    private:
        glm::vec2 GetUv(glm::vec3 position);
};
}
}
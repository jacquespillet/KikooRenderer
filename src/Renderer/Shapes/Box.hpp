#pragma once
#include "Shape.hpp"
#include "Geometry/Ray.hpp"
#include "../Materials/Material.hpp"

namespace KikooRenderer {
namespace OfflineRenderer {

class Material;
class Box : public Shape {
    public:
        glm::vec3 position;
        glm::vec3 size;
        glm::mat4 invTransf;
        
        Material* material;
        Box(glm::vec3 position, glm::vec3 size, Material* material);
        double HitRay(KikooRenderer::Geometry::Ray ray, double tMin, double tMax, Point& hitPoint);
        glm::vec3 GetPosition(double time);
        glm::vec3 GetSize(double time);
};
}
}
#pragma once
#include "Shape.hpp"
#include "Geometry/Ray.hpp"
#include "../Materials/Material.hpp"

namespace KikooRenderer {
namespace OfflineRenderer {

class Material;
class Aggregate : public Shape {
    public:
        
        Aggregate(glm::vec3 position,  Material* material);
        double HitRay(KikooRenderer::Geometry::Ray ray, double tMin, double tMax, Point& hitPoint);
        glm::vec3 GetPosition(double time);

    private:
        glm::vec3 position;
        glm::vec3 size;
        glm::mat4 transf;
        glm::mat4 invTransf;
        glm::vec2 GetUv(glm::vec3 position);
};
}
}
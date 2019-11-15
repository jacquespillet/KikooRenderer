#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"
#include "../Materials/Material.hpp"

namespace KikooRenderer {
namespace OfflineRenderer {

class Material;

struct Point {
    double t;
    glm::vec3 position;
    glm::vec3 normal;
    Material* material;
    glm::vec2 uv;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

class Shape {
    public: 
        Shape(){}
        virtual double HitRay(KikooRenderer::Geometry::Ray ray, double tMin, double tMax, Point& hitPoint) = 0;
        virtual glm::vec3 GetPosition(double time) = 0;
        virtual void GetWorldBounds(glm::vec3& min, glm::vec3& max);
};
}
}
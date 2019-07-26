#pragma once
#include "Common/Common.h"
#include "Geometry/Ray.hpp"
#include "Material.hpp"

namespace KikooRenderer {
namespace Renderer {

class Material;

struct Point {
    double t;
    glm::dvec3 position;
    glm::dvec3 normal;
    Material* material;
};

//TODO
    //scatter when hit

class Shape {
    public: 
        Shape(){}
        virtual double HitRay(KikooRenderer::Geometry::Ray ray, double tMin, double tMax, Point& hitPoint) = 0;
        virtual glm::dvec3 GetPosition(double time) = 0;
};


class Sphere : public Shape {
    public:
        glm::dvec3 position;
        double radius;
        Material material;
        Sphere(glm::dvec3 position, double radius, Material material);
        double HitRay(KikooRenderer::Geometry::Ray ray, double tMin, double tMax, Point& hitPoint);
        glm::dvec3 GetPosition(double time);
};
}
}
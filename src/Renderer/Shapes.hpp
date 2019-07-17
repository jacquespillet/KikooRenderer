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
    //Add material to shape
    //Set point material at shape.hit
    //Add material to shape constructor
    //scatter when hit

class Shape {
    public: 
        Shape(){}
        virtual double HitRay(KikooRenderer::Geometry::Ray ray, double tMin, double tMax, Point& hitPoint) = 0;
};


class Sphere : public Shape {
    public:
        glm::dvec3 position;
        double radius;
        // Material material;
        // Sphere(glm::dvec3 position, double radius, Material material);
        Sphere(glm::dvec3 position, double radius);
        double HitRay(KikooRenderer::Geometry::Ray ray, double tMin, double tMax, Point& hitPoint);
};
}
}
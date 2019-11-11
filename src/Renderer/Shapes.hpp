#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"
#include "Materials/Material.hpp"

namespace KikooRenderer {
namespace OfflineRenderer {

class Material;

struct Point {
    double t;
    glm::vec3 position;
    glm::vec3 normal;
    Material* material;
};

//TODO
    //scatter when hit

class Shape {
    public: 
        Shape(){}
        virtual double HitRay(KikooRenderer::Geometry::Ray ray, double tMin, double tMax, Point& hitPoint) = 0;
        virtual glm::vec3 GetPosition(double time) = 0;
};


class Sphere : public Shape {
    public:
        glm::vec3 position;
        double radius;
        Material* material;
        Sphere(glm::vec3 position, double radius, Material* material);
        double HitRay(KikooRenderer::Geometry::Ray ray, double tMin, double tMax, Point& hitPoint);
        glm::vec3 GetPosition(double time);
};
}
}
#include "Shapes.hpp"

namespace KikooRenderer {
namespace OfflineRenderer {
    Sphere::Sphere(glm::vec3 position, double radius, Material material) : material(material){
        this->position = position;
        this->radius = radius;
    };

    glm::vec3 Sphere::GetPosition(double time) {
        //If moving : set position at time
        return this->position;
    }


    double Sphere::HitRay(KikooRenderer::Geometry::Ray ray, double tMin, double tMax, Point& hitPoint) {
        glm::vec3 oc = ray.origin - GetPosition(ray.time);
        float a = glm::dot(ray.direction, ray.direction);
        float b = 2 * dot(oc, ray.direction);
        float c = dot(oc, oc) - radius * radius;
        float delta = b*b - 4 * a * c;
        if(delta < 0) {
            return -1;
        } else {
            double t = (- b - sqrt(delta)) / (2 * a);
            if(t > tMin && t < tMax) {
                glm::vec3 hitPosition = ray.pointAtPosition(t);
                hitPoint = {
                    t, 
                    hitPosition,
                    glm::normalize(hitPosition -  GetPosition(ray.time)),
                    &material
                };
                return t;
            }
            t = (- b + sqrt(delta)) / (2 * a);
            if(t > tMin && t < tMax) {
                glm::vec3 hitPosition = ray.pointAtPosition(t);
                hitPoint = {
                    t, 
                    hitPosition,
                    glm::normalize(hitPosition -  GetPosition(ray.time)),
                    &material
                };
                return t;
            }
        }
        return -1;
    }
}
}
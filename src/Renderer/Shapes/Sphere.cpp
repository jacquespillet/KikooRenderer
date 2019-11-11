#include "Sphere.hpp"

namespace KikooRenderer {
namespace OfflineRenderer {
    Sphere::Sphere(glm::vec3 position, double radius, Material* material) : material(material){
        this->position = position;
        this->radius = radius;
    };

    glm::vec3 Sphere::GetPosition(double time) {
        //If moving : set position at time
        return this->position;
    }

    glm::vec2 Sphere::GetUv(glm::vec3 p) {
        float phi = std::atan2(p.z, p.x);
        float theta = std::asin(p.y);

        float u = 1.0f - (phi + PI) / (2.0f * PI);
        float v = (theta + PI/2.0f) / PI;

        return glm::vec2(u, v);
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
                glm::vec2 uv = GetUv(hitPosition);
                hitPoint = {
                    t, 
                    hitPosition,
                    glm::normalize(hitPosition -  GetPosition(ray.time)),
                    material,
                    uv
                };
                return t;
            }
            t = (- b + sqrt(delta)) / (2 * a);
            if(t > tMin && t < tMax) {
                glm::vec3 hitPosition = ray.pointAtPosition(t);
                glm::vec2 uv = GetUv(hitPosition);
                hitPoint = {
                    t, 
                    hitPosition,
                    glm::normalize(hitPosition -  GetPosition(ray.time)),
                    material,
                    uv
                };
                return t;
            }
        }
        return -1;
    }
}
}
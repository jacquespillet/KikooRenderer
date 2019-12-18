#include "Lambertian.hpp"
#include "../Shapes/Shape.hpp"
#include "Geometry/Util.h"


namespace KikooRenderer{
namespace OfflineRenderer {

    Lambertian::Lambertian(glm::vec4 albedo) : Material(albedo), albedo(albedo) {}
    bool Lambertian::Scatter(KikooRenderer::Geometry::Ray in,  Point point, glm::vec3& attenuation, KikooRenderer::Geometry::Ray& scattered, float& pdf) {
        glm::vec3 outNormal;
        glm::vec3 reflected = glm::reflect(in.direction, point.normal);
        double niOverNt;
        attenuation = glm::vec3(1.0, 1.0, 1.0);
        glm::vec3 refracted;
        double probability;
        double cosine;
        if (glm::dot(in.direction, point.normal) > 0) {
            outNormal = -point.normal;
            niOverNt = refInx;
            cosine = refInx * glm::dot(in.direction, point.normal) / in.direction.length();
        }
        else {
            outNormal = point.normal;
            niOverNt = 1.0 / refInx;
            cosine = -glm::dot(in.direction, point.normal) / in.direction.length();
        }
        if (Geometry::Refract(in.direction, outNormal, niOverNt, refracted)) {
            probability = Geometry::Schlick(cosine, refInx);
        }
        else {
            scattered = Geometry::Ray(point.position, reflected, in.time);
            probability = 1.0;
        }
        double random = ((double) rand()) / (double) RAND_MAX;
        if (random < probability) {
            scattered = Geometry::Ray(point.position, reflected, in.time);
        }
        else {
            scattered = Geometry::Ray(point.position, refracted, in.time);
        }
        return true;
    }
}
}
#include "Material.hpp"
#include "Shapes.hpp"
#include "Geometry/Util.h"


namespace KikooRenderer{
namespace OfflineRenderer {

    Material::Material(glm::dvec4 albedo, bool metallic) : albedo(albedo), metallic(metallic) {}
    bool Material::Scatter(KikooRenderer::Geometry::Ray in,  Point point, glm::dvec3& attenuation, KikooRenderer::Geometry::Ray& scattered) {
        //Lambertian
        if(albedo.a < 1.0) {
             glm::dvec3 outNormal;
             glm::dvec3 reflected = glm::reflect(in.direction, point.normal);
             double niOverNt;
             attenuation = glm::dvec3(1.0, 1.0, 1.0);
             glm::dvec3 refracted;
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
        } else if(!metallic) {
            glm::dvec3 target = point.position + point.normal + Geometry::randomInSphere();
            scattered = Geometry::Ray(point.position, target-point.position);
            attenuation = albedo;
            return true;
        } else {
            // double fuzz = 0;
            //Metallic
            glm::dvec3 reflected = glm::reflect(glm::normalize(in.direction), glm::normalize(point.normal));
            scattered = Geometry::Ray(point.position, reflected + fuzz * Geometry::randomInSphere());
            attenuation = albedo;
            return glm::dot(scattered.direction, point.normal) > 0;
        }
    }
}
}
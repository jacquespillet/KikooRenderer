#include "Metallic.hpp"
#include "../Shapes.hpp"
#include "Geometry/Util.h"


namespace KikooRenderer{
namespace OfflineRenderer {

    Metallic::Metallic(glm::vec4 albedo) : albedo(albedo) {}
    bool Metallic::Scatter(KikooRenderer::Geometry::Ray in,  Point point, glm::vec3& attenuation, KikooRenderer::Geometry::Ray& scattered) {
        glm::vec3 reflected = glm::reflect(glm::normalize(in.direction), glm::normalize(point.normal));
        scattered = Geometry::Ray(point.position, reflected + fuzz * Geometry::randomInSphere());
        attenuation = albedo;
        return glm::dot(scattered.direction, point.normal) > 0;
    }
}
}
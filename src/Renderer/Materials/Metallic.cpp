#include "Metallic.hpp"
#include "../Shapes/Shape.hpp"
#include "Geometry/Util.h"


namespace KikooRenderer{
namespace OfflineRenderer {

    Metallic::Metallic(glm::vec4 albedo) : Material(albedo), albedo(albedo) {}
    bool Metallic::Scatter(KikooRenderer::Geometry::Ray in,  Point point,  ScatterRecord& scatterRecord) {
        glm::vec3 reflected = glm::reflect(glm::normalize(in.direction), glm::normalize(point.normal));
        scatterRecord.specularRay = Geometry::Ray(point.position, reflected + fuzz * Geometry::randomInSphere());
        scatterRecord.attenuation = albedo;
        scatterRecord.isSpecular = true;
        scatterRecord.pdf = nullptr;
        return true;
    }
}
}
#include "Material.hpp"
#include "../Shapes.hpp"
#include "Geometry/Util.h"


namespace KikooRenderer{
namespace OfflineRenderer {

    Material::Material(glm::vec4 albedo) : albedo(albedo) {}
    bool Material::Scatter(KikooRenderer::Geometry::Ray in,  Point point, glm::vec3& attenuation, KikooRenderer::Geometry::Ray& scattered) {
        glm::vec3 target = point.position + point.normal + Geometry::randomInSphere();
        scattered = Geometry::Ray(point.position, target-point.position);
        attenuation = albedo;
        return true;
    }
}
}
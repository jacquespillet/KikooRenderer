#include "Material.hpp"
#include "Shapes.hpp"
#include "Geometry/Util.h"


namespace KikooRenderer{
namespace Renderer {

    Material::Material(glm::dvec4 albedo) : albedo(albedo) {}
    bool Material::Scatter(KikooRenderer::Geometry::Ray in,  Point point, glm::dvec3 attenuation, KikooRenderer::Geometry::Ray scattered) {
        std::cout << " point in " << glm::to_string(point.position) << std::endl;

        glm::dvec3 target = point.position + point.normal + Geometry::randomInSphere();
        scattered = Geometry::Ray(point.position, target-point.position);
        attenuation = albedo;
        return true;
    }

}
}
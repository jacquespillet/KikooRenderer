#include "Material.hpp"

namespace KikooRenderer{
namespace Renderer {

    glm::dvec3 randomInSphere() {
        glm::dvec3 result;
        do {
            double randomX = ((double) rand()) / (double) RAND_MAX;
            double randomY = ((double) rand()) / (double) RAND_MAX;
            double randomZ = ((double) rand()) / (double) RAND_MAX;
            result = 2.0 * glm::dvec3(randomX, randomY, randomZ) - glm::dvec3(1, 1, 1);
        } while(glm::length(result) > 1);

        return result;
    }

    Material::Material(glm::dvec4 albedo) : albedo(albedo) {}
    bool Material::Scatter(KikooRenderer::Geometry::Ray in,  Point point, glm::dvec3 attenuation, KikooRenderer::Geometry::Ray scattered) {
        glm::dvec3 target = point.position + point.normal + randomInSphere();
        scattered = KikooRenderer::Geometry::Ray(point.position, target-point.position);
        attenuation = albedo;
        return true;
    }

}
}
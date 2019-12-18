#include "Metallic.hpp"
#include "../Shapes/Shape.hpp"
#include "Geometry/Util.h"


namespace KikooRenderer{
namespace OfflineRenderer {

    Metallic::Metallic(glm::vec4 albedo) : Material(albedo), albedo(albedo) {}
    bool Metallic::Scatter(KikooRenderer::Geometry::Ray in,  Point point, glm::vec3& attenuation, KikooRenderer::Geometry::Ray& scattered, float& pdf) {
        glm::vec3 reflected = glm::reflect(glm::normalize(in.direction), glm::normalize(point.normal));
        scattered = Geometry::Ray(point.position, reflected + fuzz * Geometry::randomInSphere());
        
        if(hasTexture) {
            float texCoordX = std::min((float)width, std::max(0.0f, point.uv.x * (float)width));
            float texCoordY = std::min((float)width, std::max(0.0f, point.uv.y * (float)height));

            attenuation.r = (float)(textureData[std::floor(texCoordY * width + texCoordX)]) / 255;
            attenuation.g = (float)(textureData[std::floor(texCoordY * width + texCoordX + 1)]) / 255;
            attenuation.b = (float)(textureData[std::floor(texCoordY * width + texCoordX + 2)]) / 255;
        } else {
            attenuation = albedo;
        }        
        return glm::dot(scattered.direction, point.normal) > 0;
    }
}
}
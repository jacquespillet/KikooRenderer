#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"
#include "Material.hpp"

namespace KikooRenderer{
namespace OfflineRenderer {
struct Point;
    
class Metallic : public Material {
    public: 
        // Metallic(){}
        Metallic(glm::vec4 albedo);
        glm::vec4 albedo;
        bool Scatter(KikooRenderer::Geometry::Ray in,  Point point, glm::vec3& attenuation, KikooRenderer::Geometry::Ray& scattered) override;

        double fuzz = 0;
};

}
}
#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"
#include "Material.hpp"

namespace KikooRenderer{
namespace OfflineRenderer {
struct Point;
    
class Lambertian : public Material {
    public: 
        // Lambertian() : Material(){}
        Lambertian(glm::vec4 albedo);
        glm::vec4 albedo;
        bool Scatter(KikooRenderer::Geometry::Ray in,  Point point, ScatterRecord& scatterRecord) override;
        double refInx = 1.5;
};

}
}
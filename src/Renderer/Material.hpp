#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"

namespace KikooRenderer{
namespace OfflineRenderer {
struct Point;
    
class Material {
    public: 
        Material(){}
        Material(glm::vec4 albedo, bool metallic);
        glm::vec4 albedo;
        bool Scatter(KikooRenderer::Geometry::Ray in,  Point point, glm::vec3& attenuation, KikooRenderer::Geometry::Ray& scattered);

        bool metallic = false;
        double refInx = 1.5;
        double fuzz = 0;
};

}
}
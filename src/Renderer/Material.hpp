#pragma once
#include "Common/Common.h"
#include "Geometry/Ray.hpp"

namespace KikooRenderer{
namespace Renderer {
struct Point;
    
class Material {
    public: 
        Material(){}
        Material(glm::dvec4 albedo, bool metallic);
        glm::dvec4 albedo;
        bool Scatter(KikooRenderer::Geometry::Ray in,  Point point, glm::dvec3& attenuation, KikooRenderer::Geometry::Ray& scattered);

        bool metallic = false;
        double refInx = 1.5;
        double fuzz = 0;
};

}
}
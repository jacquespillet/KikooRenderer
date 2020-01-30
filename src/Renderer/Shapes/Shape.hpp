#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"
#include "../Materials/Material.hpp"
#include "Renderer/Common.hpp"

namespace KikooRenderer {
namespace OfflineRenderer {

class Material;

class Shape {
    public: 
        Material* material;
        
        Shape(){}

        ~Shape() {
            delete material;
        }
        virtual double HitRay(KikooRenderer::Geometry::Ray ray, double tMin, double tMax, Point& hitPoint) = 0;
        virtual glm::vec3 GetPosition(double time) = 0;
        virtual void GetWorldBounds(glm::vec3& min, glm::vec3& max);

        virtual float pdfValue(glm::vec3 origin, glm::vec3 direction, std::vector<Shape*>& shapes) {
            return 0;
        }

        virtual glm::vec3 random(glm::vec3 origin) {
            return glm::vec3(0);
        }
};
}
}
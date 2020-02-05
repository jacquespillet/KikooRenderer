#pragma once

#include "Util/Common.h"
#include "Shapes/Shape.hpp"
#include "Geometry/Ray.hpp"

namespace KikooRenderer{
namespace OfflineRenderer {
class RayTracer {
    
    public: 
        RayTracer();
        void SetScene();
        void WriteImage();
        glm::vec3 GetColor(KikooRenderer::Geometry::Ray ray, int depth, const std::vector<glm::vec2>& brdfSamples, int currentSample);

        std::vector<Shape*> objects;
    // private:
};
}
}
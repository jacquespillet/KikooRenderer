#pragma once

#include "Util/Common.h"
#include "Shapes.hpp"
#include "Geometry/Ray.hpp"

namespace KikooRenderer{
namespace Renderer {
class RayTracer {
    
    public: 
        RayTracer();
        void SetScene();
        void WriteImage();
        glm::dvec3 GetColor(KikooRenderer::Geometry::Ray ray, int depth);

    private:
        std::vector<Shape*> objects;
};
}
}
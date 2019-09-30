#pragma once

#include "Util/Common.h"
#include "Shapes.hpp"
#include "Geometry/Ray.hpp"

namespace KikooRenderer{
namespace OfflineRenderer {
class RayTracer {
    
    public: 
        RayTracer();
        void SetScene();
        void WriteImage();
        glm::vec3 GetColor(KikooRenderer::Geometry::Ray ray, int depth);

    private:
        std::vector<Shape*> objects;
};
}
}
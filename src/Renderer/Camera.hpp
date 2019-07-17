#pragma once
#include "Common/Common.h"
#include "Geometry/Ray.hpp"

namespace KikooRenderer{
namespace Renderer {

class Camera {
    public: 
        Camera();
        KikooRenderer::Geometry::Ray GetRay(double u, double v);

    private:
        glm::dvec3 position;
        glm::dvec3 lowerLeft;
        glm::dvec3 horizontal;
        glm::dvec3 vertical;

};

}
}
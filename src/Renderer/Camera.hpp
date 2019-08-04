#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"

namespace KikooRenderer{
namespace Renderer {

class Camera {
    public: 
        Camera(glm::dvec3 lookfrom, glm::dvec3 lookat, glm::dvec3 up, double fovy, double aspectRatio, double aperture, double focusDistance, double t0, double t1);
        KikooRenderer::Geometry::Ray GetRay(double u, double v);

    private:
        glm::dvec3 position;
        glm::dvec3 lowerLeft;
        glm::dvec3 horizontal;
        glm::dvec3 vertical;

        glm::dvec3 u, v, w;
        
        double lensRadius;

        double time0, time1;

};

}
}
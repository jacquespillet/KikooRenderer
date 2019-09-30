#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"

namespace KikooRenderer{
namespace OfflineRenderer {

class Camera {
    public: 
        Camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 up, double fovy, double aspectRatio, double aperture, double focusDistance, double t0, double t1);
        KikooRenderer::Geometry::Ray GetRay(double u, double v);

    private:
        glm::vec3 position;
        glm::vec3 lowerLeft;
        glm::vec3 horizontal;
        glm::vec3 vertical;

        glm::vec3 u, v, w;
        
        double lensRadius;

        double time0, time1;

};

}
}
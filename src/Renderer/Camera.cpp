#include "Camera.hpp"
#include "Geometry/Util.h"

namespace KikooRenderer{
namespace OfflineRenderer {

    Camera::Camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 up, double fovy, double aspectRatio, double aperture, double focusDistance, double t0, double t1) {
        time0 = t0;
        time1 = t1;
        lensRadius = aperture / 2.0;

        float theta = fovy * M_PI / 180;
        float halfHeight = std::tan(theta/2);
        float halfWidth = aspectRatio *  halfHeight;

        position = lookfrom;
        
        w = glm::normalize(position - lookat);
        u = glm::normalize(glm::cross(up, w));
        v = glm::cross(w, u);

        lowerLeft = position - halfWidth * (float)focusDistance * u - halfHeight *(float)focusDistance * v - (float)focusDistance *w;
        horizontal = 2 * halfWidth * (float)focusDistance * u;
        vertical = 2 * halfHeight * (float)focusDistance *v;
        
    }

    KikooRenderer::Geometry::Ray Camera::GetRay(double s, double t) {
        glm::vec3 rd = (float)lensRadius * Geometry::RandomInDisk();
        glm::vec3 offset =  rd.x * u  + rd.y * v;
        double time = time0 + ((double) rand() / (double) RAND_MAX) * (time1 - time0);
        return KikooRenderer::Geometry::Ray();
        // return KikooRenderer::Geometry::Ray(position + offset, glm::normalize(lowerLeft + s * horizontal + (float)t*vertical - position - offset), time);
    }


}
}
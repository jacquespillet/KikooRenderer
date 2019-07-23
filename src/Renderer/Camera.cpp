#include "Camera.hpp"
#include "Geometry/Util.h"

namespace KikooRenderer{
namespace Renderer {

    Camera::Camera(glm::dvec3 lookfrom, glm::dvec3 lookat, glm::dvec3 up, double fovy, double aspectRatio, double aperture, double focusDistance) {
        lensRadius = aperture / 2.0;

        double theta = fovy * M_PI / 180;
        double halfHeight = std::tan(theta/2);
        double halfWidth = aspectRatio *  halfHeight;

        position = lookfrom;
        
        w = glm::normalize(position - lookat);
        u = glm::normalize(glm::cross(up, w));
        v = glm::cross(w, u);

        lowerLeft = position - halfWidth * focusDistance * u - halfHeight *focusDistance * v - focusDistance *w;
        horizontal = 2 * halfWidth * focusDistance * u;
        vertical = 2 * halfHeight * focusDistance *v;
        
    }

    KikooRenderer::Geometry::Ray Camera::GetRay(double s, double t) {
        glm::dvec3 rd = lensRadius * Geometry::RandomInDisk();
        glm::dvec3 offset =  rd.x * u  + rd.y * v;

        return KikooRenderer::Geometry::Ray(position + offset, lowerLeft + s * horizontal + t*vertical - position - offset);
    }


}
}
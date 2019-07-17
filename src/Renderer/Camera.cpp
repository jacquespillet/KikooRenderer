#include "Camera.hpp"

namespace KikooRenderer{
namespace Renderer {

    Camera::Camera() {
        lowerLeft = glm::dvec3 (-2, -1, -1);
        horizontal = glm::dvec3 (4, 0, 0);
        vertical = glm::dvec3 (0, 2, 0);
        position = glm::dvec3 (0, 0, 0);
    }

    KikooRenderer::Geometry::Ray Camera::GetRay(double u, double v) {
        return KikooRenderer::Geometry::Ray(position, lowerLeft + u * horizontal + v*vertical);
    }


}
}
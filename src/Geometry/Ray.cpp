#include "Ray.hpp"

namespace KikooRenderer {
namespace Geometry {
    Ray::Ray(glm::dvec3 origin, glm::dvec3 direction) {
        this->origin = origin;
        this->direction = direction;
    }


    glm::dvec3 Ray::pointAtPosition(double t) {
        return origin + t * direction;
    }

}
}
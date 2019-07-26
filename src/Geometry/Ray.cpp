#include "Ray.hpp"

namespace KikooRenderer {
namespace Geometry {
    Ray::Ray(glm::dvec3 origin, glm::dvec3 direction, float time) {
        this->origin = origin;
        this->direction = direction;
        this->time = time;
    }


    glm::dvec3 Ray::pointAtPosition(double t) {
        return origin + t * direction;
    }

}
}
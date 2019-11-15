#include "Ray.hpp"

namespace KikooRenderer {
namespace Geometry {
    Ray::Ray(glm::vec3 origin, glm::vec3 direction, float time) {
        this->origin = origin;
        // this->direction = glm::normalize(direction);
        this->direction = direction;
        this->time = time;
    }


    glm::vec3 Ray::pointAtPosition(double t) {
        return origin + t * direction;
    }

}
}
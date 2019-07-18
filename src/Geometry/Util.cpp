#include "Util.h"

namespace KikooRenderer{
namespace Geometry {
    glm::dvec3 randomInSphere(){
        glm::dvec3 result;
        do {
            double randomX = ((double) rand()) / (double) RAND_MAX;
            double randomY = ((double) rand()) / (double) RAND_MAX;
            double randomZ = ((double) rand()) / (double) RAND_MAX;
            result = 2.0 * glm::dvec3(randomX, randomY, randomZ) - glm::dvec3(1, 1, 1);
        } while(glm::length(result) > 1);

        return result;
    }
}
}
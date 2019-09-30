#include "Util.h"

namespace KikooRenderer{
namespace Geometry {
    glm::vec3 randomInSphere(){
        glm::vec3 result;
        do {
            double randomX = ((double) rand()) / (double) RAND_MAX;
            double randomY = ((double) rand()) / (double) RAND_MAX;
            double randomZ = ((double) rand()) / (double) RAND_MAX;
            result = 2.0 * glm::vec3(randomX, randomY, randomZ) - glm::vec3(1, 1, 1);
        } while(glm::length(result) > 1);

        return result;
    }

    glm::vec3 RandomInDisk() {
        glm::vec3 p;
        do {
            double randomX = ((double) rand()) / (double) RAND_MAX;
            double randomY = ((double) rand()) / (double) RAND_MAX;
            p = 2.0 * glm::vec3(randomX, randomY, 0) - glm::vec3(1, 1, 0);
        } while(glm::dot(p, p) >= 1.0);
        return p;
    }


    double Schlick(double cosine, double inx) {
        double r0 = (1.0 - inx) / (1.0 + inx);
        r0 = r0 * r0;
        return r0 + (1.0-r0) * std::pow((1.0 - cosine), 5);
    }

    bool Refract(glm::vec3 v, glm::vec3 n, double niOverNt, glm::vec3& refracted) {
        glm::vec3 uv = glm::normalize(v);

        double dt = glm::dot(uv, n);
        double delta = 1.0 - niOverNt * niOverNt * (1.0 - dt*dt);
        if(delta > 0) {
            refracted = niOverNt * (uv - n * dt) - n * std::sqrt(delta);
            return true;
        } else {
            return false;
        }
    }


}
}
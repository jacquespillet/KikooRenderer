#include "Util.h"

namespace KikooRenderer{
namespace Geometry {

    double RandomInRange(double a, double b) {
        double random = ((double) rand()) / (double) (RAND_MAX+1);
        random = a + random * (b - a);

        return random;
    }   

    glm::vec3 randomInSphere(){
        glm::vec3 result(1000);
        do {
            double randomX = ((double) rand()) / (double) (RAND_MAX+1);
            double randomY = ((double) rand()) / (double) (RAND_MAX+1);
            double randomZ = ((double) rand()) / (double) (RAND_MAX+1);
            result = 2.0f * glm::vec3(randomX, randomY, randomZ) - glm::vec3(1, 1, 1);
        } while(glm::dot(result, result) > 1);

        return result;
    }

    glm::vec3 randomOnSphere(){
        glm::vec3 result(1000);
        do {
            double randomX = ((double) rand()) / (double) (RAND_MAX+1);
            double randomY = ((double) rand()) / (double) (RAND_MAX+1);
            double randomZ = ((double) rand()) / (double) (RAND_MAX+1);
            result = 2.0f * glm::vec3(randomX, randomY, randomZ) - glm::vec3(1, 1, 1);
        } while(glm::dot(result, result) > 1);

        return glm::normalize(result);
    }    

    glm::vec3 RandomCosineDirection() {
        double r1 = ((double) rand()) / (double) (RAND_MAX+1);
        double r2 = ((double) rand()) / (double) (RAND_MAX+1);
        double z = sqrt(1-r2);
        double phi = 2*M_PI*r1;
        double x = cos(phi)*sqrt(r2);
        double y = sin(phi)*sqrt(r2);
        return glm::vec3(x, y, z);
    }

    glm::vec3 RandomCosineDirection(float r1, float r2) {
        double z = sqrt(1-r2);
        double phi = 2*M_PI*r1;
        double x = cos(phi)*sqrt(r2);
        double y = sin(phi)*sqrt(r2);
        return glm::vec3(x, y, z);
    }

    glm::vec3 RandomInDisk() {
        glm::vec3 p;
        do {
            double randomX = ((double) rand()) / (double) (RAND_MAX+1);
            double randomY = ((double) rand()) / (double) RAND_MAX;
            p = 2.0f * glm::vec3(randomX, randomY, 0) - glm::vec3(1, 1, 0);
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

        float dt = glm::dot(uv, n);
        float delta = 1.0 - niOverNt * niOverNt * (1.0 - dt*dt);
        if(delta > 0) {            refracted = (float) niOverNt * (uv - n * dt) - n * std::sqrt(delta);
            return true;
        } else {
            return false;
        }
    }


}
}
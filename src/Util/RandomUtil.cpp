#include "RandomUtil.hpp"

namespace KikooRenderer {
namespace Util {
float GetRand() {
    return ((float)std::rand() / (float)(RAND_MAX+1));
}

float GenerateValue(float average, float errorMargin) {
    float offset = (GetRand() - 0.5f) * 2.0f * errorMargin;
    return average + offset;
}

glm::vec3 GenerateRandomUnitVector() {
    float theta = (float) (GetRand() * 2.0f * PI);
    float z = (GetRand() * 2) - 1;
    float rootOneMinusZSquared = (float) std::sqrt(1 - z * z);
    float x = (float) (rootOneMinusZSquared * std::cos(theta));
    float y = (float) (rootOneMinusZSquared * std::sin(theta));
    return  glm::vec3(x, y, z);
}

glm::vec3 GenerateRandomUnitVectorWithinCone(glm::vec3 coneDirection, float angle) {
    float cosAngle = (float) std::cos(angle);
    float theta = (float) (GetRand() * 2.0f * PI);
    float z = cosAngle + (GetRand() * (1 - cosAngle));
    float rootOneMinusZSquared = (float) std::sqrt(1 - z * z);
    float x = (float) (rootOneMinusZSquared * std::cos(theta));
    float y = (float) (rootOneMinusZSquared * std::sin(theta));

    glm::vec4 direction(x, y, z, 1);
    if (coneDirection.x != 0 || coneDirection.y != 0 || (coneDirection.z != 1 && coneDirection.z != -1)) {
        glm::vec3 rotateAxis = glm::cross(coneDirection, glm::vec3(0, 0, 1));
        glm::normalize(rotateAxis);
        float rotateAngle = (float) std::acos(glm::dot(coneDirection, glm::vec3(0, 0, 1)));
        
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0), -rotateAngle, rotateAxis);
        direction = rotationMatrix * direction;
    } else if (coneDirection.z == -1) {
        direction.z *= -1;
    }
    return glm::vec3(direction);
}

glm::vec4 GetRandomColor() {
    return glm::vec4(GetRand(), GetRand(), GetRand(), 1);
}

}
}
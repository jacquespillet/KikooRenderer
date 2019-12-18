#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"
#include "Geometry/Util.h"

namespace KikooRenderer {
namespace OfflineRenderer {

class CosinePdf {
public: 
    CosinePdf(const glm::mat4& _transform) { this->transform = _transform; }
    virtual float value(const glm::vec3& direction) {
        glm::vec3 normal =  glm::normalize(glm::vec3(glm::column(transform, 2)));
        float cosine = glm::dot(glm::normalize(direction),normal);
        if(cosine < 0) cosine=0;
        return cosine / PI;
    }

    virtual glm::vec3 generate() const {
        glm::vec4 randomDirection = glm::vec4(Geometry::RandomCosineDirection(), 0);
        glm::vec3 direction = glm::vec3(transform * randomDirection);
        return direction;
    }

private:
    glm::mat4 transform;
};
}
}

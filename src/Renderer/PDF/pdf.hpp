#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"

namespace KikooRenderer {
namespace OfflineRenderer {

class Pdf {
public: 
    virtual float value(const glm::vec3& direction) const=0;
    virtual glm::vec3 generate() const=0;
};
}
}

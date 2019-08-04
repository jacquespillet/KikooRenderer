#pragma once

#include "Util/Common.h"


namespace KikooRenderer {
namespace Geometry {
class Box {
    public: 
        Box();
        glm::dvec3 position;
        glm::dvec3 size;
};
}
}
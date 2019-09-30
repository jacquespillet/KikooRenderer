#pragma once

#include "Util/Common.h"


namespace KikooRenderer {
namespace Geometry {
class Box {
    public: 
        Box();
        glm::vec3 position;
        glm::vec3 size;
};
}
}
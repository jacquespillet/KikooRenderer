#pragma once
#include "Util/Common.h"

namespace KikooRenderer {
namespace OfflineRenderer {

class Material;

struct Point {
    double t;
    glm::vec3 position;
    glm::vec3 normal;
    Material* material;
    glm::vec2 uv;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

}
}
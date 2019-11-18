#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"

namespace KikooRenderer{
namespace OfflineRenderer {
class Cubemap {
public:
    Cubemap();
    Cubemap(std::vector<std::string> files);
    glm::vec4 Sample(glm::vec3 uvw);

private:
    std::vector<std::string> files;
    std::vector<std::vector<uint8_t>> texturesData;
    int width, height, nrChannels;
    enum FACE {RIGHT, LEFT, TOP, BOTTOM, BACK, FRONT};
};
}
}
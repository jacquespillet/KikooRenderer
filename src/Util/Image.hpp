#pragma once

#include "Common/Common.h"

namespace KikooRenderer {
namespace Util {
namespace FileIO {
class Image {
    public:
        Image(int width, int height, glm::dvec4 color = glm::dvec4(0, 0, 0, 0));
        void SetPixel(int x, int y, glm::dvec4 color);
        void SetPixel(int x, int y, glm::dvec3 color);
        void SetPixel(int x, int y, float r, float g, float b, float a);
        void toPPM(std::string fileName);
        void toPNG(std::string fileName);
        void toJPG(std::string fileName);

    private:
        std::vector<glm::dvec4> colorBuffer;
        int width;
        int height;
};
}
}
}
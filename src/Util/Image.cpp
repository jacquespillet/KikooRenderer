#include "Image.hpp"

namespace KikooRenderer {
namespace Util {
namespace FileIO {

    Image::Image(int width, int height, glm::dvec4 color) {
        this->width = width;
        this->height = height;
        colorBuffer = std::vector<glm::dvec4>(width * height, color);
    }

    void Image::toPPM(std::string fileName) {
        std::fstream fs (fileName, std::fstream::out);
        fs << "P3\n" << width << " " << height << "\n255\n";
        for(int i=0; i<colorBuffer.size(); i++) {
            int r = colorBuffer[i].x * 255;
            int g = colorBuffer[i].y * 255;
            int b = colorBuffer[i].z * 255;

            fs << r << " " << g << " " << b << "\n";

        }
        fs.close();
    }

    void Image::SetPixel(int x, int y, glm::dvec4 color) {
        int inx = y * width + x;
        colorBuffer[inx] = color;
    }

    void Image::SetPixel(int x, int y, glm::dvec3 color) {
        int inx = y * width + x;
        colorBuffer[inx] = glm::dvec4(color, 1);
    }
}
}
}

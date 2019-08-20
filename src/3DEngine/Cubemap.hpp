#pragma once

#include "Util/Common.h"

namespace KikooRenderer {
namespace CoreEngine {

class Cubemap {
    public:
    unsigned int textureID;
    GLint texIndex;
    int width, height;
    bool loaded = false; 
    Cubemap(std::vector<std::string> files);  
    Cubemap() {loaded = false;}
    
};

}
}
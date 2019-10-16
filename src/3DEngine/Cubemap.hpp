#pragma once

#include "Util/Common.h"

namespace KikooRenderer {
namespace CoreEngine {

class Cubemap {
    public:
        Cubemap(std::vector<std::string> files);  
        Cubemap() {loaded = false;}
        void Use();

        unsigned int GetTextureID();
        void  SetTextureID(unsigned int value);
        GLint GetTexIndex();
        void  SetTexIndex(GLint value);
        int GetWidth();
        void SetWidth(int value);
        int GetHeight();
        void SetHeight(int value);
        bool GetLoaded();
        void  SetLoaded(bool value);

    protected:    
        unsigned int textureID;
        GLint texIndex;
        int width, height;
        bool loaded = false; 
};

}
}
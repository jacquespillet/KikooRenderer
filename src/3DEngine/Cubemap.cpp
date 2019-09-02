#include "Cubemap.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

// #define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace KikooRenderer {
namespace CoreEngine {

Cubemap::Cubemap(std::vector<std::string> files) {
    GETGL
    ogl->glGenTextures(1, &textureID);
    ogl->glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0); 
    ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0); 
    
    int width, height, nrChannels;
    for (unsigned int i = 0; i < files.size(); i++)
    {
        unsigned char *data = stbi_load(files[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            ogl->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << files[i] << std::endl;
            stbi_image_free(data);
        }
    }
    

    loaded = true;
    ogl->glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::Use() {
    GETGL
    // std::cout << textureID << std::endl;    
    ogl->glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

}
}
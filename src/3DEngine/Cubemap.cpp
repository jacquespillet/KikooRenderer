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
    std::cout << "0"<<std::endl;
    GETGL
    std::cout << "1"<<std::endl;
    ogl->glGenTextures(1, &textureID);
    ogl->glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    unsigned char *data;  
    int numChannels;
    //Order : Right - Left - Top - bottom - back - front
    for(GLuint i = 0; i < files.size(); i++)
    {
        data = stbi_load(files[i].c_str(), &width, &height, &numChannels, 0);
		GLint texType = (numChannels == 3) ? GL_RGB : GL_RGBA;        
        ogl->glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
            0, texType, width, height, 0, texType, GL_UNSIGNED_BYTE, data
        );
    }    

    ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
}

}
}
#include "CubeFramebuffer.hpp"
#include "Components/MaterialComponent.hpp"

namespace KikooRenderer {
namespace CoreEngine {

CubeFramebuffer::CubeFramebuffer(int width, int height , int internalColorFormat , int colorFormat , int colorType , bool hasColor, bool hasDepth) {
    GETGL
    this->width = width;
    this->height = height;
    this->colorFormat = colorFormat;

    ogl->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);

    ogl->glGenFramebuffers(1, &fbo);
    ogl->glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    if(hasDepth) {
        ogl->glGenTextures(1, &depthCubemap);
        ogl->glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

        for (unsigned int i = 0; i < 6; ++i)
                ogl->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        
        ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
    
        ogl->glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
        ogl->glDrawBuffer(GL_NONE);
        ogl->glReadBuffer(GL_NONE);

        const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    }

    if(hasColor) {

    }

    ogl->glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
}

//Directly renders the object to the target object
void CubeFramebuffer::RenderOnObect(std::vector<Object3D*>& objectsToRender, Object3D* target) {
}

void CubeFramebuffer::RenderOnObect(Object3D* objectToRender, Object3D* target) {

}


//Render object to the current FBO
void CubeFramebuffer::RenderObjectsToFBO(std::vector<Object3D*>& objectsToRender) {

}

void CubeFramebuffer::RenderObjectToFBO(Object3D* objectToRender) {

}


//Render the FBO to a target object	
void CubeFramebuffer::RenderFBOToObject(Object3D* target, bool renderDepth) {

}



Cubemap* CubeFramebuffer::GetColorTexture() {
    return nullptr;
}

Cubemap* CubeFramebuffer::GetDepthTexture() {
    return nullptr;
}



}
}
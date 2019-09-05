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
    // create depth cubemap texture
    ogl->glGenTextures(1, &depthCubemap);
    ogl->glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (unsigned int i = 0; i < 6; ++i)
        ogl->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // attach depth texture as FBO's depth buffer
    ogl->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    ogl->glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    ogl->glDrawBuffer(GL_NONE);
    ogl->glReadBuffer(GL_NONE);
    
    // ogl->glGenFramebuffers(1, &fbo);
    // ogl->glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // if(hasDepth) {
    //     ogl->glGenTextures(1, &depthCubemap);
    //     ogl->glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

    //     for (unsigned int i = 0; i < 6; ++i)
    //             ogl->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        
    //     ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //     ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //     ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //     ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //     ogl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
    
    //     ogl->glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    //     ogl->glDrawBuffer(GL_NONE);
    //     ogl->glReadBuffer(GL_NONE);

    //     const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    // }

    // if(hasColor) {

    // }
    
    // ogl->glEnable(GL_CULL_FACE); 
    // ogl->glCullFace(GL_BACK);

    // ogl->glEnable(GL_BLEND);
    // ogl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // ogl->glEnable(GL_STENCIL_TEST);    
    // ogl->glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  
    
    // ogl->glEnable(GL_MULTISAMPLE);  

    // //disable writting to depth buffer
    // ogl->glEnable(GL_DEPTH_TEST);
    
    // // now that we actually created the fbo and added all attachments we want to check if it is actually complete now
    // if (ogl->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
    //     std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << width << "  " << height << std::endl;
    // } else {
    //     std::cout << "Succeffuly created FBO"<<std::endl;
    // }

    // ogl->glBindRenderbuffer(GL_RENDERBUFFER, 0);


    ogl->glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
}


void CubeFramebuffer::Enable() {
    GETGL
    ogl->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
    ogl->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void CubeFramebuffer::Disable() {
    GETGL
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
#include "LightComponent.hpp"
#include "TransformComponent.hpp"
#include "../Scene.hpp"


#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {
namespace CoreEngine {

LightComponent::LightComponent(Object3D* object, glm::dvec4 color, glm::dvec3 attenuation, int type) : Component("Light", object) {
    this->type = type;
    this->attenuation =  attenuation;
    this->color = color;   

    depthFBO = new Framebuffer(1024, 1024, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, true, true);
    lightProjection  = glm::orthoLH(-10.0, 10.0, -10.0, 10.0, 1.0, 100.0);
}

void LightComponent::RenderDepthMap() {
    GETGL
    ogl->glViewport(0, 0, 1024, 1024);
    depthFBO->Enable(); 
    ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
    ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);

    glm::dmat4 viewMat = glm::inverse(object3D->transform->GetWorldModelMatrix());
    for(int i=0; i<object3D->scene->objects3D.size(); i++) {
        if(object3D->scene->objects3D[i] && object3D->scene->objects3D[i]->visible ) {
            object3D->scene->objects3D[i]->Render(&viewMat, &lightProjection); 
        }
    }
    lightSpaceMatrix = lightProjection * viewMat; 
    depthFBO->Disable();

}

void LightComponent::OnStart(){}
void LightComponent::OnEnable(){}
void LightComponent::OnUpdate(){}
void LightComponent::OnRender(){} 
void LightComponent::OnDestroy(){} 
void LightComponent::Recompute(){} 

}
}
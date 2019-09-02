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

    object3D->scene->glWindow->makeCurrent();
    depthFBO = new Framebuffer(1024, 1024, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, true, true);
    
    
    lightProjection  = glm::orthoLH(-10.0, 10.0, -10.0, 10.0, 1.0, 10.0);

    depthPassShader.vertSrc= R"(
    #version 440
    layout(location = 0) in vec3 position;

    uniform mat4 modelViewProjectionMatrix;
    void main()
    {
        gl_Position = modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);
    }
    )";

    depthPassShader.fragSrc = R"(
    #version 440
    layout(location = 0) out vec4 outputColor; 
    void main()
    {   
        outputColor = vec4(0);
    }
    )";
    depthPassShader.name = "quad";
    std::cout << "StandardShaders: Compiling depthPassShader" << std::endl; 
    depthPassShader.Compile();
    object3D->scene->glWindow->doneCurrent();
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
            //USE UNLIT LIKE SHADER HERE INSTEAD OF CURRENT SHADER
            //Check how we swap shaders in forward renderer selection shader
            MaterialComponent* material = (MaterialComponent*) object3D->scene->objects3D[i]->GetComponent("Material");
            if(material != nullptr) {
                Shader* tmpShader = material->shader;
                ShaderParams* tmpParams = material->params;
                material->SetShader(&depthPassShader);
                object3D->scene->objects3D[i]->Render(&viewMat, &lightProjection); 
                material->SetShader(tmpShader);
                material->params = tmpParams;
            }
            
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
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
    
    //Avant dernier arg TRUE pour debug, doit etre FALSE
    depthFBO = new Framebuffer(1024, 1024, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, false, true);
    depthCubeFBO = new CubeFramebuffer(1024, 1024, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, true, true);
    
    
    lightProjection  = glm::orthoLH(-20.0, 20.0, -20.0, 20.0, 1.0, 100.0);

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

    
    cubeDepthPassShader.vertSrc= R"(
    #version 440
    layout(location = 0) in vec3 position;

    uniform mat4 modelViewProjectionMatrix;
    void main()
    {
        gl_Position = modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);
    }
    )";

    cubeDepthPassShader.fragSrc = R"(
    #version 440
    layout(location = 0) out vec4 outputColor; 
    void main()
    {   
        outputColor = vec4(0);
    }
    )";
    cubeDepthPassShader.name = "quad";
    std::cout << "StandardShaders: Compiling cubeDepthPassShader" << std::endl; 
    cubeDepthPassShader.Compile();


    object3D->scene->glWindow->doneCurrent();
}

void LightComponent::RenderDepthMap() {
    GETGL;

    ogl->glViewport(0, 0, 1024, 1024);
    if(type==0) {
        depthFBO->Enable(); 
        ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
        ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);

        glm::dmat4 model = object3D->transform->GetWorldRotationMatrix();
        glm::dvec3 lightPos = -20.0 * glm::dvec4(glm::column(model, 2));
        model = glm::translate(model, lightPos);
        glm::dmat4 viewMat = glm::inverse(model);

        for(int i=0; i<object3D->scene->objects3D.size(); i++) {
            if(object3D->scene->objects3D[i] && object3D->scene->objects3D[i]->visible ) {
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
    } else if(type == 1) {
        float aspect = (float)1024/(float)1024;
        float nearPlane = 1.0f;
        float farPlane = 25.0f;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, nearPlane, farPlane);    

        glm::vec3 lightPos = glm::vec3(object3D->transform->position);
        lightSpaceMatrices.push_back(shadowProj * 
                        glm::lookAt(lightPos, lightPos + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
        lightSpaceMatrices.push_back(shadowProj * 
                        glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
        lightSpaceMatrices.push_back(shadowProj * 
                        glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
        lightSpaceMatrices.push_back(shadowProj * 
                        glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
        lightSpaceMatrices.push_back(shadowProj * 
                        glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
        lightSpaceMatrices.push_back(shadowProj * 
                        glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)));

        
    }

}

void LightComponent::OnStart(){}
void LightComponent::OnEnable(){}
void LightComponent::OnUpdate(){}
void LightComponent::OnRender(){} 
void LightComponent::OnDestroy(){} 
void LightComponent::Recompute(){} 

}
}
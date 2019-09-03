#include "LightComponent.hpp"
#include "TransformComponent.hpp"
#include "../Scene.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {
namespace CoreEngine {

LightViewBuilder::LightViewBuilder(LightComponent* light) {
    this->light = light;

    UP = glm::dvec4(0, 1, 0, 0);
    FORWARD = glm::dvec4(0, 0, 1, 0);
    MAX_DISTANCE = 100;
    OFFSET = 10;

    farHeight = 0;
    farWidth = 0;
    nearHeight = 0;
    nearWidth = 0;

    min = glm::dvec3(9999);
    max = glm::dvec3(-9999);

    lightViewMatrix = glm::dmat4(1);
}

void LightViewBuilder::Update() {
        glm::dmat4 rotation = calculateCameraRotationMatrix();
        glm::dvec3 forwardVector = rotation * FORWARD;
 
        glm::dvec3 toFar = forwardVector * MAX_DISTANCE;
        glm::dvec3 toNear = forwardVector * 0.0001;

        glm::dvec3 centerNear = toNear + light->object3D->scene->camera->transform->position;
        glm::dvec3 centerFar = toFar + light->object3D->scene->camera->transform->position;
 
        std::vector<glm::dvec4> points = calculateFrustumVertices(rotation, forwardVector, centerNear,
                centerFar);
 
        boolean first = true;
        for (glm::dvec4 point : points) {
            if (first) {
                min.x = point.x;
                max.x = point.x;
                min.y = point.y;
                max.y = point.y;
                min.z = point.z;
                max.z = point.z;
                first = false;
                continue;
            }

            if (point.x > max.x) {
                max.x = point.x;
            } else if (point.x < min.x) {
                min.x = point.x;
            }

            if (point.y > max.y) {
                max.y = point.y;
            } else if (point.y < min.y) {
                min.y = point.y;
            }

            if (point.z > max.z) {
                max.z = point.z;
            } else if (point.z < min.z) {
                min.z = point.z;
            }
        }
        max.z += OFFSET;    
}


void LightViewBuilder::calculateWidthsAndHeights(){
    double aspectRatio = (double)light->object3D->scene->windowWidth / (double)light->object3D->scene->windowHeight; 
    farWidth = (double) (MAX_DISTANCE * std::tan(DEGTORAD * light->object3D->scene->camera->fov));
    nearWidth = (double) (0.001 * std::tan(DEGTORAD * light->object3D->scene->camera->fov));
    farHeight = farWidth / aspectRatio;
    nearHeight = nearWidth / aspectRatio;
}

glm::dmat4 LightViewBuilder::calculateCameraRotationMatrix(){
    glm::dmat4 res(1);
    res  = glm::rotate(res, light->object3D->scene->camera->transform->rotation.y * DEGTORAD, glm::dvec3(0.0, 1.0, 0.0));//rot y axis
    res  = glm::rotate(res, light->object3D->scene->camera->transform->rotation.x * DEGTORAD, glm::dvec3(1.0, 0.0, 0.0));//rot x axis
    return res;
}

glm::dvec4 LightViewBuilder::calculateLightSpaceFrustumCorner(glm::dvec3 startPoint, glm::dvec3 direction,float width){
    glm::dvec4 res(0);

    return res;
}

std::vector<glm::dvec4> LightViewBuilder::calculateFrustumVertices(glm::dmat4 rotation, glm::dvec3 forwardVector, glm::dvec3 centerNear, glm::dvec3 centerFar){
    std::vector<glm::dvec4> res;
    glm::dvec3 centerNear, glm::dvec3 centerFar) {
    glm::dvec3 upVector = new glm::dvec3(Matrix4f.transform(rotation, UP, null));
    glm::dvec3 rightVector = glm::dvec3.cross(forwardVector, upVector, null);
    glm::dvec3 downVector = new glm::dvec3(-upVector.x, -upVector.y, -upVector.z);
    glm::dvec3 leftVector = new glm::dvec3(-rightVector.x, -rightVector.y, -rightVector.z);
    glm::dvec3 farTop = glm::dvec3.add(centerFar, new glm::dvec3(upVector.x * farHeight,
            upVector.y * farHeight, upVector.z * farHeight), null);
    glm::dvec3 farBottom = glm::dvec3.add(centerFar, new glm::dvec3(downVector.x * farHeight,
            downVector.y * farHeight, downVector.z * farHeight), null);
    glm::dvec3 nearTop = glm::dvec3.add(centerNear, new glm::dvec3(upVector.x * nearHeight,
            upVector.y * nearHeight, upVector.z * nearHeight), null);
    glm::dvec3 nearBottom = glm::dvec3.add(centerNear, new glm::dvec3(downVector.x * nearHeight,
            downVector.y * nearHeight, downVector.z * nearHeight), null);
    Vector4f[] points = new Vector4f[8];
    points[0] = calculateLightSpaceFrustumCorner(farTop, rightVector, farWidth);
    points[1] = calculateLightSpaceFrustumCorner(farTop, leftVector, farWidth);
    points[2] = calculateLightSpaceFrustumCorner(farBottom, rightVector, farWidth);
    points[3] = calculateLightSpaceFrustumCorner(farBottom, leftVector, farWidth);
    points[4] = calculateLightSpaceFrustumCorner(nearTop, rightVector, nearWidth);
    points[5] = calculateLightSpaceFrustumCorner(nearTop, leftVector, nearWidth);
    points[6] = calculateLightSpaceFrustumCorner(nearBottom, rightVector, nearWidth);
    points[7] = calculateLightSpaceFrustumCorner(nearBottom, leftVector, nearWidth);
    return points;
    return res;
}


LightComponent::LightComponent(Object3D* object, glm::dvec4 color, glm::dvec3 attenuation, int type) : Component("Light", object), lightViewBuilder(this) {
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

    lightViewBuilder.Update(); 

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
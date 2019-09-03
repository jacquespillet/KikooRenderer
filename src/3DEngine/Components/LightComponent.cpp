#include "LightComponent.hpp"
#include "TransformComponent.hpp"
#include "../Scene.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {
namespace CoreEngine {

// LightViewBuilder::LightViewBuilder(LightComponent* light) {
//     this->light = light;

//     UP = glm::dvec4(0, 1, 0, 0);
//     FORWARD = glm::dvec4(0, 0, 1, 0);
//     MAX_DISTANCE = 100;
//     OFFSET = 10;

//     farHeight = 0;
//     farWidth = 0;
//     nearHeight = 0;
//     nearWidth = 0;

//     min = glm::dvec3(9999);
//     max = glm::dvec3(-9999);

//     lightViewMatrix = glm::dmat4(1);
    
//     calculateWidthsAndHeights();
// }

// void LightViewBuilder::Update() {
//         glm::dmat4 rotation = calculateCameraRotationMatrix();
//         glm::dvec3 forwardVector = rotation * FORWARD;
 
//         glm::dvec3 toFar = forwardVector * MAX_DISTANCE;
//         glm::dvec3 toNear = forwardVector * 0.0001;

//         glm::dvec3 centerNear = toNear + light->object3D->scene->camera->transform->position;
//         glm::dvec3 centerFar = toFar + light->object3D->scene->camera->transform->position;
 
//         std::vector<glm::dvec4> points = calculateFrustumVertices(rotation, forwardVector, centerNear,
//                 centerFar);
 
//         boolean first = true;
//         for (glm::dvec4 point : points) {
//             if (first) {
//                 min.x = point.x;
//                 max.x = point.x;
//                 min.y = point.y;
//                 max.y = point.y;
//                 min.z = point.z;
//                 max.z = point.z;
//                 first = false;
//                 continue;
//             }

//             if (point.x > max.x) {
//                 max.x = point.x;
//             } else if (point.x < min.x) {
//                 min.x = point.x;
//             }

//             if (point.y > max.y) {
//                 max.y = point.y;
//             } else if (point.y < min.y) {
//                 min.y = point.y;
//             }

//             if (point.z > max.z) {
//                 max.z = point.z;
//             } else if (point.z < min.z) {
//                 min.z = point.z;
//             }
//         }
//         max.z += OFFSET;    
// }


// void LightViewBuilder::calculateWidthsAndHeights(){
//     double aspectRatio = (double)light->object3D->scene->windowWidth / (double)light->object3D->scene->windowHeight; 
//     farWidth = (double) (MAX_DISTANCE * std::tan(DEGTORAD * light->object3D->scene->camera->fov));
//     nearWidth = (double) (0.001 * std::tan(DEGTORAD * light->object3D->scene->camera->fov));
//     farHeight = farWidth / aspectRatio;
//     nearHeight = nearWidth / aspectRatio;
// }

// glm::dmat4 LightViewBuilder::calculateCameraRotationMatrix(){
//     glm::dmat4 res(1);
//     res  = glm::rotate(res, light->object3D->scene->camera->transform->rotation.y * DEGTORAD, glm::dvec3(0.0, 1.0, 0.0));//rot y axis
//     res  = glm::rotate(res, light->object3D->scene->camera->transform->rotation.x * DEGTORAD, glm::dvec3(1.0, 0.0, 0.0));//rot x axis
//     return res;
// }

// glm::dvec4 LightViewBuilder::calculateLightSpaceFrustumCorner(glm::dvec3 startPoint, glm::dvec3 direction,float width){
//     glm::dvec3 point = startPoint + glm::dvec3(direction.x * width, direction.y * width, direction.z * width);
//     glm::dvec4 point4f(point.x, point.y, point.z, 1);
//     lightViewMatrix * point4f;
//     return point4f;
// }

// std::vector<glm::dvec4> LightViewBuilder::calculateFrustumVertices(glm::dmat4 rotation, glm::dvec3 forwardVector, glm::dvec3 centerNear, glm::dvec3 centerFar){
//     glm::dvec3 upVector = rotation * UP;
//     glm::dvec3 rightVector = glm::cross(forwardVector, upVector);
//     glm::dvec3 downVector(-upVector.x, -upVector.y, -upVector.z);
//     glm::dvec3 leftVector(-rightVector.x, -rightVector.y, -rightVector.z);
//     glm::dvec3 farTop = centerFar + glm::dvec3(upVector.x * farHeight,upVector.y * farHeight, upVector.z * farHeight);
//     glm::dvec3 farBottom = centerFar +  glm::dvec3(downVector.x * farHeight, downVector.y * farHeight, downVector.z * farHeight);
//     glm::dvec3 nearTop = centerNear + glm::dvec3(upVector.x * nearHeight, upVector.y * nearHeight, upVector.z * nearHeight);
//     glm::dvec3 nearBottom = centerNear + glm::dvec3(downVector.x * nearHeight, downVector.y * nearHeight, downVector.z * nearHeight);
    
//     std::vector<glm::dvec4> points(8);
//     points[0] = calculateLightSpaceFrustumCorner(farTop, rightVector, farWidth);
//     points[1] = calculateLightSpaceFrustumCorner(farTop, leftVector, farWidth);
//     points[2] = calculateLightSpaceFrustumCorner(farBottom, rightVector, farWidth);
//     points[3] = calculateLightSpaceFrustumCorner(farBottom, leftVector, farWidth);
//     points[4] = calculateLightSpaceFrustumCorner(nearTop, rightVector, nearWidth);
//     points[5] = calculateLightSpaceFrustumCorner(nearTop, leftVector, nearWidth);
//     points[6] = calculateLightSpaceFrustumCorner(nearBottom, rightVector, nearWidth);
//     points[7] = calculateLightSpaceFrustumCorner(nearBottom, leftVector, nearWidth);
//     return points;
// }

// glm::dvec3 LightViewBuilder::GetCenter() {
//     float x = (min.x + max.x) / 2.0;
//     float y = (min.y + max.y) / 2.0;
//     float z = (min.z + max.z) / 2.0;
//     glm::dvec4 cen(x, y, z, 1);
//     cen =
//     Matrix4f invertedLight = new Matrix4f();
//     Matrix4f.invert(lightViewMatrix, invertedLight);
//     return new Vector3f(Matrix4f.transform(invertedLight, cen, null));
// }

// glm::dmat4 GetProjection() {

// }

// glm::dmat4  LightViewBuilder::GetView() {
//     glm::dvec3 direction = glm::column(light->object3D->transform->GetWorldModelMatrix(), 2); 
//     center.negate();
//     // lightViewMatrix.setIdentity();
//     // float pitch = (float) Math.acos(new Vector2f(direction.x, direction.z).length());
//     // Matrix4f.rotate(pitch, new Vector3f(1, 0, 0), lightViewMatrix, lightViewMatrix);
//     // float yaw = (float) Math.toDegrees(((float) Math.atan(direction.x / direction.z)));
//     // yaw = direction.z > 0 ? yaw - 180 : yaw;
//     // Matrix4f.rotate((float) -Math.toRadians(yaw), new Vector3f(0, 1, 0), lightViewMatrix,
//     //         lightViewMatrix);
//     // Matrix4f.translate(center, lightViewMatrix, lightViewMatrix);
// }



LightComponent::LightComponent(Object3D* object, glm::dvec4 color, glm::dvec3 attenuation, int type) : Component("Light", object) {
    this->type = type;
    this->attenuation =  attenuation;
    this->color = color;   

    object3D->scene->glWindow->makeCurrent();
    depthFBO = new Framebuffer(1024, 1024, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, true, true);
    
    
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
    object3D->scene->glWindow->doneCurrent();
}

void LightComponent::RenderDepthMap() {
    GETGL;

    ogl->glViewport(0, 0, 1024, 1024);
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

}

void LightComponent::OnStart(){}
void LightComponent::OnEnable(){}
void LightComponent::OnUpdate(){}
void LightComponent::OnRender(){} 
void LightComponent::OnDestroy(){} 
void LightComponent::Recompute(){} 

}
}
#include "BoundingComponent.hpp"
#include "MeshFilterComponent.hpp"
#include "../BaseObjects.hpp"


#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {
namespace CoreEngine {
    BoundingBoxComponent::BoundingBoxComponent(Object3D* object) : Component("BoundingBox", object) {
    }
   
    void BoundingBoxComponent::OnStart() {    }
   
    void BoundingBoxComponent::OnEnable() {
        inited = false;
        min = glm::dvec3(9999, 9999, 9999);
        max = glm::dvec3(-9999, -9999, -9999);
        MeshFilterComponent* mesh = (MeshFilterComponent*) this->object3D->GetComponent("MeshFilter");
        transform = (TransformComponent*) this->object3D->GetComponent("Transform");
        

        for(int i=0; i<mesh->vertices.size(); i++) {
            if(mesh->vertices[i].position.x < min.x) min.x = mesh->vertices[i].position.x; 
            if(mesh->vertices[i].position.y < min.y) min.y = mesh->vertices[i].position.y; 
            if(mesh->vertices[i].position.z < min.z) min.z = mesh->vertices[i].position.z; 
            
            if(mesh->vertices[i].position.x > max.x) max.x = mesh->vertices[i].position.x; 
            if(mesh->vertices[i].position.y > max.y) max.y = mesh->vertices[i].position.y; 
            if(mesh->vertices[i].position.z > max.z) max.z = mesh->vertices[i].position.z;
        }
        
        
        
        
        
        glm::dvec3 position = (max + min) / 2.0;
        glm::dvec3 scale = max - min;
        boxObject = GetWireFrameBox(object3D->scene, "BoundingBox", position, glm::dvec3(0), scale, glm::dvec4(0, 1, 0, 1));
        inited = true;
    }
    
    void BoundingBoxComponent::OnUpdate() {
    }
    
    void BoundingBoxComponent::OnRender() {

    }
    
    void BoundingBoxComponent::OnDestroy() {

    }
    
    Object3D* BoundingBoxComponent::GetBoxObject() {
        return boxObject;
    }

    void BoundingBoxComponent::GetWorldBounds(glm::dvec3* _min, glm::dvec3* _max) {
        (*_min) = this->transform->GetModelMatrix() * glm::dvec4(this->min, 1.0);
        (*_max) = this->transform->GetModelMatrix() * glm::dvec4(this->max, 1.0);
    }

    void BoundingBoxComponent::GetLocalBounds(glm::dvec3* _min, glm::dvec3* _max) {
        (*_min) =  glm::dvec4(this->min, 1.0);
        (*_max) =  glm::dvec4(this->max, 1.0);
    }
}
}